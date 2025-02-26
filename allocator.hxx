/*!
  \file allocator.hxx
  \brief Файл содержит объявление класса allocator.
*/

#ifndef allocator_hxx
#define allocator_hxx

#include <cstddef>
#include <mutex>
#include <unordered_map>

class allocator {
private:
  std::size_t                                          _size_block{}, _blocks_amount{}, _size_storage{};
  std::byte *                                          _storage{};
  decltype( _storage )                                 _pointer{};
  std::unordered_map< std::byte *, const std::size_t > _left_borders{};
  std::mutex                                           _mutex{};

public:
  allocator( std::size_t size_block = 1u, std::size_t blocks_amount = 1u );
  ~allocator();
  auto take( std::size_t blocks_amount ) -> decltype( _storage );
  auto release( std::byte * block_pointer ) -> void;
};

#endif
