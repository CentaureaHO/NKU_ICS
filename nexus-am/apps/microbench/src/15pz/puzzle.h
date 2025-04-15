// Author:  Douglas Wilhelm Harder
// Copyright (c) 2009 by Douglas Wilhelm Harder.  All rights reserved.
// Url: https://ece.uwaterloo.ca/~dwharder/aads/Algorithms/N_puzzles/
#include <benchmark.h>
#include <klib.h>

<<<<<<< HEAD
template <int N>
class N_puzzle
{
  private:
    bool    puzzle_valid;
    uint8_t zero_i, zero_j;
    int8_t  manhattan_distance;
    int8_t  puzzle[N][N];
    int     hash_value;
=======
template <int N> class N_puzzle {
private:
  bool puzzle_valid;
  uint8_t zero_i, zero_j;
  int8_t manhattan_distance;
  int8_t puzzle[N][N];
  int hash_value;
>>>>>>> master

  void determine_hash();

<<<<<<< HEAD
    static int abs(int n) { return (n < 0) ? -n : n; }

  public:
    N_puzzle();
    N_puzzle(int array[N * N]);
    N_puzzle(N_puzzle const&);
    N_puzzle& operator=(N_puzzle const&);

    bool         solvable() const;
    bool         valid() const;
    int          lower_bound() const;
    unsigned int hash() const;
=======
  static int abs(int n) { return (n < 0) ? -n : n; }

public:
  N_puzzle();
  N_puzzle(int array[N * N]);
  N_puzzle(N_puzzle const &);
  N_puzzle &operator=(N_puzzle const &);

  bool solvable() const;
  bool valid() const;
  int lower_bound() const;
  unsigned int hash() const;
>>>>>>> master

  bool tile_up_possible() const;
  bool tile_down_possible() const;
  bool tile_left_possible() const;
  bool tile_right_possible() const;

  N_puzzle tile_up() const;
  N_puzzle tile_down() const;
  N_puzzle tile_left() const;
  N_puzzle tile_right() const;

<<<<<<< HEAD
    bool operator==(N_puzzle const&) const;
    bool operator!=(N_puzzle const&) const;
=======
  bool operator==(N_puzzle const &) const;
  bool operator!=(N_puzzle const &) const;
>>>>>>> master

  N_puzzle static solution();
};

template <int N>
<<<<<<< HEAD
N_puzzle<N>::N_puzzle() : puzzle_valid(true), manhattan_distance(0)
{
    int array[N * N];

    for (int i = 0; i < N * N; ++i) {
        array[i] = i;
=======
N_puzzle<N>::N_puzzle() : puzzle_valid(true), manhattan_distance(0) {
  int array[N * N];

  for (int i = 0; i < N * N; ++i) {
    array[i] = i;
  }

  int n = 0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      int k = bench_rand() % (N * N - n);
      puzzle[i][j] = array[k];

      if (array[k] == 0) {
        zero_i = i;
        zero_j = j;
      } else {
        manhattan_distance += abs(((array[k] - 1) / N) - i);
        manhattan_distance += abs(((array[k] - 1) % N) - j);
      }

      ++n;
      array[k] = array[N * N - n];
>>>>>>> master
    }

    int n = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int k        = bench_rand() % (N * N - n);
            puzzle[i][j] = array[k];

            if (array[k] == 0) {
                zero_i = i;
                zero_j = j;
            }
            else
            {
                manhattan_distance += abs(((array[k] - 1) / N) - i);
                manhattan_distance += abs(((array[k] - 1) % N) - j);
            }

            ++n;
            array[k] = array[N * N - n];
        }
    }

    determine_hash();
}

template <int N>
<<<<<<< HEAD
N_puzzle<N>::N_puzzle(int array[N * N]) : puzzle_valid(true), manhattan_distance(0)
{
    bool check[N * N];

    for (int i = 0; i < N * N; ++i) {
        check[i] = false;
=======
N_puzzle<N>::N_puzzle(int array[N * N])
    : puzzle_valid(true), manhattan_distance(0) {
  bool check[N * N];

  for (int i = 0; i < N * N; ++i) {
    check[i] = false;
  }

  int n = 0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      puzzle[i][j] = array[n];
      check[array[n]] = true;

      if (array[n] == 0) {
        zero_i = i;
        zero_j = j;
      } else {
        manhattan_distance += abs(((array[n] - 1) / N) - i);
        manhattan_distance += abs(((array[n] - 1) % N) - j);
      }

      ++n;
>>>>>>> master
    }

<<<<<<< HEAD
    int n = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            puzzle[i][j]    = array[n];
            check[array[n]] = true;

            if (array[n] == 0) {
                zero_i = i;
                zero_j = j;
            }
            else
            {
                manhattan_distance += abs(((array[n] - 1) / N) - i);
                manhattan_distance += abs(((array[n] - 1) % N) - j);
            }

            ++n;
        }
=======
  for (int i = 0; i < N * N; ++i) {
    if (!check[i]) {
      puzzle_valid = false;
      return;
>>>>>>> master
    }

    for (int i = 0; i < N * N; ++i) {
        if (!check[i]) {
            puzzle_valid = false;
            return;
        }
    }

    determine_hash();
}

/*
 * Determine a hash value for the puzzle.
 */

<<<<<<< HEAD
template <int N>
void          N_puzzle<N>::determine_hash()
{
    hash_value = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            hash_value = hash_value * 1973 + puzzle[i][j];
        }
=======
template <int N> void N_puzzle<N>::determine_hash() {
  hash_value = 0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      hash_value = hash_value * 1973 + puzzle[i][j];
>>>>>>> master
    }
}

template <int N>
<<<<<<< HEAD
N_puzzle<N>::N_puzzle(N_puzzle const& pz)
    : puzzle_valid(pz.puzzle_valid),
      zero_i(pz.zero_i),
      zero_j(pz.zero_j),
      manhattan_distance(pz.manhattan_distance),
      hash_value(pz.hash_value)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            puzzle[i][j] = pz.puzzle[i][j];
        }
=======
N_puzzle<N>::N_puzzle(N_puzzle const &pz)
    : puzzle_valid(pz.puzzle_valid), zero_i(pz.zero_i), zero_j(pz.zero_j),
      manhattan_distance(pz.manhattan_distance), hash_value(pz.hash_value) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      puzzle[i][j] = pz.puzzle[i][j];
>>>>>>> master
    }
}

<<<<<<< HEAD
template <int N>
N_puzzle<N>& N_puzzle<N>::operator=(N_puzzle const& rhs)
{
    puzzle_valid       = rhs.puzzle_valid;
    zero_i             = rhs.zero_i;
    zero_j             = rhs.zero_j;
    manhattan_distance = rhs.manhattan_distance;
    hash_value         = rhs.hash_value;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            puzzle[i][j] = rhs.puzzle[i][j];
        }
=======
template <int N> N_puzzle<N> &N_puzzle<N>::operator=(N_puzzle const &rhs) {
  puzzle_valid = rhs.puzzle_valid;
  zero_i = rhs.zero_i;
  zero_j = rhs.zero_j;
  manhattan_distance = rhs.manhattan_distance;
  hash_value = rhs.hash_value;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      puzzle[i][j] = rhs.puzzle[i][j];
>>>>>>> master
    }
    return *this;
}

/*
 *  Moving a tile up is possible as long as
 *  the blank is not in the last row.
 */

<<<<<<< HEAD
template <int N>
bool          N_puzzle<N>::tile_up_possible() const
{
    return puzzle_valid && (zero_i != N - 1);
=======
template <int N> bool N_puzzle<N>::tile_up_possible() const {
  return puzzle_valid && (zero_i != N - 1);
>>>>>>> master
}

/*
 *  Moving a tile down is possible as long as
 *  the blank is not in the first row.
 */

<<<<<<< HEAD
template <int N>
bool          N_puzzle<N>::tile_down_possible() const
{
    return puzzle_valid && (zero_i != 0);
=======
template <int N> bool N_puzzle<N>::tile_down_possible() const {
  return puzzle_valid && (zero_i != 0);
>>>>>>> master
}

/*
 *  Moving a tile left is possible as long as
 *  the blank is not in the last column.
 */

<<<<<<< HEAD
template <int N>
bool          N_puzzle<N>::tile_left_possible() const
{
    return puzzle_valid && (zero_j != N - 1);
=======
template <int N> bool N_puzzle<N>::tile_left_possible() const {
  return puzzle_valid && (zero_j != N - 1);
>>>>>>> master
}

/*
 *  Moving a tile right is possible as long as
 *  the blank is not in the first column.
 */

<<<<<<< HEAD
template <int N>
bool          N_puzzle<N>::tile_right_possible() const
{
    return puzzle_valid && (zero_j != 0);
}

template <int N>
N_puzzle<N>   N_puzzle<N>::tile_up() const
{
    if (!puzzle_valid) {
        return *this;
    }

    N_puzzle result(*this);

    if (zero_i == N - 1) {
        result.puzzle_valid = false;
        return result;
    }

    result.manhattan_distance += abs(((puzzle[zero_i + 1][zero_j] - 1) / N) - zero_i) -
                                 abs(((puzzle[zero_i + 1][zero_j] - 1) / N) - (zero_i + 1));

    result.puzzle[zero_i][zero_j] = puzzle[zero_i + 1][zero_j];
    ++result.zero_i;
    result.puzzle[result.zero_i][zero_j] = 0;

    result.determine_hash();

    return result;
}

template <int N>
N_puzzle<N>   N_puzzle<N>::tile_down() const
{
    if (!puzzle_valid) {
        return *this;
    }

    N_puzzle result(*this);

    if (zero_i == 0) {
        result.puzzle_valid = false;
        return result;
    }

    result.manhattan_distance += abs(((puzzle[zero_i - 1][zero_j] - 1) / N) - zero_i) -
                                 abs(((puzzle[zero_i - 1][zero_j] - 1) / N) - (zero_i - 1));

    result.puzzle[zero_i][zero_j] = puzzle[zero_i - 1][zero_j];
    --result.zero_i;
    result.puzzle[result.zero_i][zero_j] = 0;

    result.determine_hash();

    return result;
}

template <int N>
N_puzzle<N>   N_puzzle<N>::tile_left() const
{
    if (!puzzle_valid) {
        return *this;
    }

    N_puzzle result(*this);

    if (zero_j == N - 1) {
        result.puzzle_valid = false;
        return result;
    }

    result.manhattan_distance += abs(((puzzle[zero_i][zero_j + 1] - 1) % N) - zero_j) -
                                 abs(((puzzle[zero_i][zero_j + 1] - 1) % N) - (zero_j + 1));

    result.puzzle[zero_i][zero_j] = puzzle[zero_i][zero_j + 1];
    ++result.zero_j;
    result.puzzle[zero_i][result.zero_j] = 0;

    result.determine_hash();

    return result;
}

template <int N>
N_puzzle<N>   N_puzzle<N>::tile_right() const
{
    if (!puzzle_valid) {
        return *this;
    }

    N_puzzle result(*this);

    if (zero_j == 0) {
        result.puzzle_valid = false;
        return result;
    }

    result.manhattan_distance += abs(((puzzle[zero_i][zero_j - 1] - 1) % N) - zero_j) -
                                 abs(((puzzle[zero_i][zero_j - 1] - 1) % N) - (zero_j - 1));

    result.puzzle[zero_i][zero_j] = puzzle[zero_i][zero_j - 1];
    --result.zero_j;
    result.puzzle[zero_i][result.zero_j] = 0;

    result.determine_hash();

    return result;
=======
template <int N> bool N_puzzle<N>::tile_right_possible() const {
  return puzzle_valid && (zero_j != 0);
}

template <int N> N_puzzle<N> N_puzzle<N>::tile_up() const {
  if (!puzzle_valid) {
    return *this;
  }

  N_puzzle result(*this);

  if (zero_i == N - 1) {
    result.puzzle_valid = false;
    return result;
  }

  result.manhattan_distance +=
      abs(((puzzle[zero_i + 1][zero_j] - 1) / N) - zero_i) -
      abs(((puzzle[zero_i + 1][zero_j] - 1) / N) - (zero_i + 1));

  result.puzzle[zero_i][zero_j] = puzzle[zero_i + 1][zero_j];
  ++result.zero_i;
  result.puzzle[result.zero_i][zero_j] = 0;

  result.determine_hash();

  return result;
}

template <int N> N_puzzle<N> N_puzzle<N>::tile_down() const {
  if (!puzzle_valid) {
    return *this;
  }

  N_puzzle result(*this);

  if (zero_i == 0) {
    result.puzzle_valid = false;
    return result;
  }

  result.manhattan_distance +=
      abs(((puzzle[zero_i - 1][zero_j] - 1) / N) - zero_i) -
      abs(((puzzle[zero_i - 1][zero_j] - 1) / N) - (zero_i - 1));

  result.puzzle[zero_i][zero_j] = puzzle[zero_i - 1][zero_j];
  --result.zero_i;
  result.puzzle[result.zero_i][zero_j] = 0;

  result.determine_hash();

  return result;
}

template <int N> N_puzzle<N> N_puzzle<N>::tile_left() const {
  if (!puzzle_valid) {
    return *this;
  }

  N_puzzle result(*this);

  if (zero_j == N - 1) {
    result.puzzle_valid = false;
    return result;
  }

  result.manhattan_distance +=
      abs(((puzzle[zero_i][zero_j + 1] - 1) % N) - zero_j) -
      abs(((puzzle[zero_i][zero_j + 1] - 1) % N) - (zero_j + 1));

  result.puzzle[zero_i][zero_j] = puzzle[zero_i][zero_j + 1];
  ++result.zero_j;
  result.puzzle[zero_i][result.zero_j] = 0;

  result.determine_hash();

  return result;
}

template <int N> N_puzzle<N> N_puzzle<N>::tile_right() const {
  if (!puzzle_valid) {
    return *this;
  }

  N_puzzle result(*this);

  if (zero_j == 0) {
    result.puzzle_valid = false;
    return result;
  }

  result.manhattan_distance +=
      abs(((puzzle[zero_i][zero_j - 1] - 1) % N) - zero_j) -
      abs(((puzzle[zero_i][zero_j - 1] - 1) % N) - (zero_j - 1));

  result.puzzle[zero_i][zero_j] = puzzle[zero_i][zero_j - 1];
  --result.zero_j;
  result.puzzle[zero_i][result.zero_j] = 0;

  result.determine_hash();

  return result;
>>>>>>> master
}

/*
 *  Check if the puzzle is solvable:  that is, check the
 *  number of inversions pluse the Manhattan distance of
 *  the black from the lower-right corner.
 *
 *  Run time:   O(n^2)
 *  Memory:     O(n)
 */

<<<<<<< HEAD
template <int N>
bool          N_puzzle<N>::solvable() const
{
    if (!valid()) {
        return false;
=======
template <int N> bool N_puzzle<N>::solvable() const {
  if (!valid()) {
    return false;
  }

  int entries[N * N];

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (puzzle[i][j] == 0) {
        entries[N * i + j] = N * N;
      } else {
        entries[N * i + j] = puzzle[i][j];
      }
>>>>>>> master
    }

    int entries[N * N];

<<<<<<< HEAD
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (puzzle[i][j] == 0) {
                entries[N * i + j] = N * N;
            }
            else
            {
                entries[N * i + j] = puzzle[i][j];
            }
        }
=======
  for (int i = 0; i < N * N; ++i) {
    for (int j = i + 1; j < N * N; ++j) {
      if (entries[i] > entries[j]) {
        ++parity;
      }
>>>>>>> master
    }

<<<<<<< HEAD
    int parity = 0;

    for (int i = 0; i < N * N; ++i) {
        for (int j = i + 1; j < N * N; ++j) {
            if (entries[i] > entries[j]) {
                ++parity;
            }
        }
    }

    parity += 2 * N - 2 - zero_i - zero_j;

    return ((parity & 1) == 0);
}

template <int N>
bool          N_puzzle<N>::valid() const
{
    return puzzle_valid;
}
=======
  parity += 2 * N - 2 - zero_i - zero_j;

  return ((parity & 1) == 0);
}

template <int N> bool N_puzzle<N>::valid() const { return puzzle_valid; }
>>>>>>> master

/*
 *  Return either the Manhattan, Hamming, or discrete distance
 *  between the puzzle and the solution.
 */

<<<<<<< HEAD
template <int N>
int           N_puzzle<N>::lower_bound() const
{
    // The Manhattan distance
    return valid() ? manhattan_distance : N * N * N;
=======
template <int N> int N_puzzle<N>::lower_bound() const {
  // The Manhattan distance
  return valid() ? manhattan_distance : N * N * N;
>>>>>>> master

    int result = 0;
    int count  = 1;

<<<<<<< HEAD
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (puzzle[i][j] != (count % N * N)) {
                ++result;
            }
=======
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (puzzle[i][j] != (count % N * N)) {
        ++result;
      }
>>>>>>> master

            ++count;
        }
    }

    // The Hamming distance, or
    return result;

    // The discrete distance:  converts the A* search to Dijkstra's algorithm
    // return ( result == 0 ) ? 0 : 1;
}

/*
 *  puzzle1 == puzzle2
 *
 *  Two puzzles are considered to be equal if their entries
 *  are equal:
 *    If either puzzle is not valid, return false.
 *    If the hash values are different, they are different; return false.
 *    Otherwise, check all entries to see if they are the same.
 */

<<<<<<< HEAD
template <int N>
bool N_puzzle<N>::operator==(N_puzzle const& rhs) const
{
    if (!valid() || !rhs.valid() || hash() != rhs.hash()) {
=======
template <int N> bool N_puzzle<N>::operator==(N_puzzle const &rhs) const {
  if (!valid() || !rhs.valid() || hash() != rhs.hash()) {
    return false;
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (puzzle[i][j] != rhs.puzzle[i][j]) {
>>>>>>> master
        return false;
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (puzzle[i][j] != rhs.puzzle[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/*
 *  puzzle1 != puzzle2
 *
 *  Two puzzles are considered to be unequal if any of the entries
 *  different:
 *    If either puzzle is not valid, return false.
 *    If the hash values are different, they are different; return true.
 *    Otherwise, check all entries to see if they are the same.
 */

<<<<<<< HEAD
template <int N>
bool N_puzzle<N>::operator!=(N_puzzle const& rhs) const
{
    if (!valid() || !rhs.valid()) {
        return false;
=======
template <int N> bool N_puzzle<N>::operator!=(N_puzzle const &rhs) const {
  if (!valid() || !rhs.valid()) {
    return false;
  }

  if (hash() != rhs.hash()) {
    return true;
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (puzzle[i][j] != rhs.puzzle[i][j]) {
        return true;
      }
>>>>>>> master
    }

    if (hash() != rhs.hash()) {
        return true;
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (puzzle[i][j] != rhs.puzzle[i][j]) {
                return true;
            }
        }
    }

    return false;
}

/*
 * unsigned int hash() const
 *
 *   Returns the pre-calculated hash value.
 */

<<<<<<< HEAD
template <int N>
unsigned int  N_puzzle<N>::hash() const
{
    return valid() ? hash_value : 0;
=======
template <int N> unsigned int N_puzzle<N>::hash() const {
  return valid() ? hash_value : 0;
>>>>>>> master
}

/*
 * N_puzzle<N>  solution()
 *
 *   Returns the correct solution to the N puzzle:
 *
 *       1  2  3         1   2   3   4
 *  3x3: 4  5  6   4x4:  5   6   7   8
 *       7  8            9  10  11  12
 *                      13  14  15
 */

<<<<<<< HEAD
template <int N>
N_puzzle<N>   N_puzzle<N>::solution()
{
    int array[N * N];

    for (int i = 0; i < N * N - 1; ++i) {
        array[i] = i + 1;
    }

    array[N * N - 1] = 0;

    return N_puzzle<N>(array);
=======
template <int N> N_puzzle<N> N_puzzle<N>::solution() {
  int array[N * N];

  for (int i = 0; i < N * N - 1; ++i) {
    array[i] = i + 1;
  }

  array[N * N - 1] = 0;

  return N_puzzle<N>(array);
>>>>>>> master
}
