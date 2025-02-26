#include <bits/stdc++.h>
#include <stdexcept>
#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include "./allocator.hxx"

TEST( allocator_test, construct ) {
  // A size of block equals to 0, an amount of blocks equals to 0.
  ASSERT_THROW( allocator( 0, 0 ), std::invalid_argument );

  // A size of block equals to 0.
  ASSERT_THROW( allocator( 0u, 681'669u ), std::invalid_argument );

  // An amount of blocks equals to 0.
  ASSERT_THROW( allocator( 95'046u, 0u ), std::invalid_argument );

  // A production size_block * blocks_amount doesn't fit 64-bit unsigned integer.
  ASSERT_THROW( allocator( std::numeric_limits< std::size_t >::max(), 2 ), std::overflow_error );
  ASSERT_THROW( allocator( 2, std::numeric_limits< std::size_t >::max() ), std::overflow_error );
  ASSERT_THROW( allocator( 5702979259955982248ull, 6424446278243694822ull ), std::overflow_error );

  // A size of block equals to 1, 1 > 0; an amount of blocks equals to 1, 1 > 0.
  ASSERT_NO_THROW( allocator() );
}

TEST( allocator_test, take_0_blocks ) {
  allocator    allocator{};
  auto * const pointer{ allocator.take( 0 ) };

  ASSERT_EQ( pointer, nullptr );
}

TEST( allocator_test, take_more_than_possible_blocks ) {
  {
    allocator    allocator{};
    auto * const pointer{ allocator.take( 1u * 1u + 1u ) };

    ASSERT_EQ( pointer, nullptr );
  }

  {
    allocator    allocator{ 1u, 10u };
    auto * const pointer{ allocator.take( 1u * 10u + 1u ) };

    ASSERT_EQ( pointer, nullptr );
  }

  {
    allocator    allocator{ 38u, 48u };
    auto * const pointer{ allocator.take( 38u * 48u + 1u ) };

    ASSERT_EQ( pointer, nullptr );
  }
}

TEST( allocator_test, release ) {
  {
    allocator allocator{};

    ASSERT_THROW( allocator.release( nullptr ), std::invalid_argument );
  }

  {
    allocator    allocator{};
    auto * const pointer_0{ allocator.take( 1u ) };

    ASSERT_NE( pointer_0, nullptr );
    ASSERT_THROW( allocator.release( nullptr ), std::invalid_argument );
    allocator.release( pointer_0 );

    auto * const pointer_1{ allocator.take( 1u ) };
    ASSERT_NE( pointer_1, nullptr );
    allocator.release( pointer_1 );
  }
}

TEST( allocator_test, take_and_release ) {
  {
    allocator allocator{};
    auto *    pointer{ allocator.take( 1u ) };

    ASSERT_NE( pointer, nullptr );
    ASSERT_EQ( allocator.take( 1u ), nullptr );
    allocator.release( pointer );
    pointer = allocator.take( 1u );
    ASSERT_NE( pointer, nullptr );
    ASSERT_EQ( allocator.take( 1u ), nullptr );
  }

  {
    allocator allocator{ 4u, 5u };
    auto *    pointer_0{ allocator.take( 2u ) };

    ASSERT_NE( pointer_0, nullptr );
    memcpy( pointer_0, "Hello!", 7u );

    auto * pointer_1{ allocator.take( 3u ) };

    ASSERT_NE( pointer_1, nullptr );
    memcpy( pointer_1, "aaBBccDDeeF", 12u );
    ASSERT_EQ( allocator.take( 1 ), nullptr );
    allocator.release( pointer_0 );
    ASSERT_EQ( allocator.take( 4u ), nullptr );
    ASSERT_EQ( allocator.take( 3u ), nullptr );

    auto * pointer_2{ allocator.take( 2u ) };

    ASSERT_NE( pointer_2, nullptr );
    ASSERT_EQ( allocator.take( 1 ), nullptr );
    ASSERT_EQ( std::memcmp( pointer_2, "Hello!", 7u ), 0 );
    ASSERT_EQ( std::memcmp( pointer_1, "aaBBccDDeeF", 12u ), 0 );
  }
}

TEST( allocator_test, several_threads ) {
  allocator                     allocator{ 8u, 32u };
  std::array< std::thread, 2u > threads{ std::thread{ [&allocator]() {
                                           std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

                                           std::array< std::byte *, 2u > pointers{};

                                           pointers.front() = allocator.take( 9u );
                                           ASSERT_NE( pointers.front(), nullptr );
                                           std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
                                           pointers.back() = allocator.take( 11u );
                                           ASSERT_NE( pointers.back(), nullptr );
                                         } },

                                         std::thread{ [&allocator]() {
                                           std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

                                           auto * const pointer{ allocator.take( 12u ) };

                                           ASSERT_NE( pointer, nullptr );

                                           std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
                                           allocator.release( pointer );
                                         } } };

  for ( auto & thread : threads ) {
    thread.join();
  }

  ASSERT_NE( allocator.take( 5u ), nullptr );
  ASSERT_NE( allocator.take( 7u ), nullptr );
  ASSERT_EQ( allocator.take( 1u ), nullptr );
}

int main( int amount_arguments, char * arguments[] ) {
  testing::InitGoogleTest( &amount_arguments, arguments );
  return RUN_ALL_TESTS();
}
