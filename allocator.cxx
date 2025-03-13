/*!
  \file allocator.cxx
  \brief В файле определяется класс allocator.
*/

#include <algorithm>
#include <cstddef>
#include <mutex>
#include <stdexcept>

#include <sys/mman.h>

#include "./allocator.hxx"

/*!
  \brief Создать аллокатор.
  \param size_block Размер блока.
  \param blocks_amount Количество выделяемых блоков.
*/
allocator::allocator( std::size_t size_block, std::size_t blocks_amount )
    : _size_block{ size_block }, _blocks_amount{ blocks_amount } {
  if ( _size_block == 0u ) {
    throw std::invalid_argument{ "A block size must be greater than 0." };
  }

  if ( _blocks_amount == 0u ) {
    throw std::invalid_argument{ "An amount of blocks must be greater than 0." };
  }

  _size_storage = _size_block * _blocks_amount;

  /* Проверка на переполнение: произведение _size_block * _blocks_amount может превышать SIZE_MAX. Произведение
  n-битных чисел есть (2n)-битное число. */
  if ( _size_block != ( _size_storage / _blocks_amount ) || _blocks_amount != ( _size_storage / _size_block ) ) {
    throw std::overflow_error{ "A production size_block * blocks_amount must be "
                               "no greater than SIZE_MAX." };
  }

  _storage = reinterpret_cast< std::byte * >(
      mmap( nullptr, _size_storage, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 ) );

  if ( _storage == MAP_FAILED ) {
    throw std::bad_alloc{};
  }

  _pointer = _storage;
}

allocator::~allocator() { munmap( _storage, _size_storage ); }

/*!
  \brief Выделить указанное число блоков памяти.
  \param blocks_amount Запрашиваемое число блоков.
*/
auto allocator::take( std::size_t blocks_amount ) -> decltype( _storage ) {
  std::lock_guard lock{ _mutex };

  if ( blocks_amount == 0u ) {
    return nullptr;
  }

  if ( blocks_amount > _blocks_amount ) {
    return nullptr;
  }

  const std::ptrdiff_t size{ _pointer - _storage };
  const auto           amount_bytes_to_allocate{ blocks_amount * _size_block };

  if ( size + amount_bytes_to_allocate > _size_storage ) {
    _pointer = _storage;
  }

  while ( _pointer + amount_bytes_to_allocate < _storage + _size_storage ) {
    const auto find_result{ _left_borders.find( _pointer ) };

    if ( find_result != _left_borders.end() ) {
      _pointer += find_result.operator*().second * _size_block;
    }

    else {
      std::byte * left_border{};

      for ( const auto & [address, length] : _left_borders ) {
        if ( address > _pointer ) {
          if ( left_border == nullptr ) {
            left_border = address;
          }

          else {
            left_border = min( address, left_border );
          }
        }
      }

      if ( left_border != nullptr ) {
        const std::ptrdiff_t length{ left_border - _pointer };

        if ( length < amount_bytes_to_allocate ) {
          _pointer = left_border + _left_borders.at( left_border );
          continue;
        }
      }

      break;
    }
  }

  if ( _pointer + amount_bytes_to_allocate > _storage + _size_storage ) {
    _pointer = _storage;
    return nullptr;
  }

  if ( _left_borders.find( _pointer ) != _left_borders.end() ) {
    _pointer = _storage;
    return nullptr;
  }

  auto pointer{ _pointer };

  _left_borders.emplace( pointer, blocks_amount );

  if ( _pointer + amount_bytes_to_allocate == _storage + _size_storage ) {
    _pointer = _storage;
  }

  else {
    _pointer += amount_bytes_to_allocate;
  }

  return pointer;
}

/*!
  \brief Освободить выделенный участок памяти.
  \param block_pointer Указатель на левую границу ранее выделенного блока.
*/
void allocator::release( std::byte * block_pointer ) {
  std::lock_guard lock{ _mutex };
  const auto      result_find{ _left_borders.find( block_pointer ) };

  if ( result_find == _left_borders.end() ) {
    throw std::invalid_argument{ "Address doesn't match a left border of a block." };
  }

  _left_borders.erase( result_find );
}
