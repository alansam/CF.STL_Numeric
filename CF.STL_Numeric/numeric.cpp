//
//  numeric.cpp
//  CF.STL_Numeric
//
//  Created by Alan Sampson on 11/11/20.
//
//  MARK: - References.
//  @see: https://en.cppreference.com/w/cpp/header/numeric
//  @see: https://www.mathsisfun.com/numbers/fibonacci-sequence.html
//  @see GMP.Factorial.Select

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <list>
#include <array>
#include <vector>
#include <forward_list>
#include <deque>
#include <set>
#include <numeric>
#include <random>
#include <span>
#include <string>
#include <functional>
#include <execution>
#include <limits>
#include <cmath>
#include <cstdint>
#include <climits>
#include <cinttypes>

using namespace std::literals::string_literals;

void fn_iota(void);
void fn_accumulate(void);
void fn_reduce(void);
void fn_transform_reduce(void);
void fn_inner_product(void);
void fn_adjacent_difference(void);
void fn_partial_sum(void);
void fn_exclusive_scan_inclusive_scan(void);
void fn_transform_exclusive_scan_transform_inclusive_scan(void);
void fn_gcd(void);
void fn_lcm(void);
void fn_midpoint(void);

/*
 *  MARK: main()
 */
int main(int argc, const char * argv[]) {
  std::cout << "CF.STL_Numeric"s << std::endl;
  std::cout << "C++ version: " << __cplusplus << std::endl;

  fn_iota();
  fn_accumulate();
  fn_reduce();
  fn_transform_reduce();
  fn_inner_product();
  fn_adjacent_difference();
  fn_partial_sum();
  fn_exclusive_scan_inclusive_scan();
  fn_transform_exclusive_scan_transform_inclusive_scan();
  fn_gcd();
  fn_lcm();
  fn_midpoint();

  return 0;
}

/*
 *  MARK: fn_iota()
 */
void fn_iota(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  std::list<int> lst(10);
  std::iota(lst.begin(), lst.end(), -4);

  std::vector<std::list<int>::iterator> vec(lst.size());
  std::iota(vec.begin(), vec.end(), lst.begin());

  std::shuffle(vec.begin(), vec.end(), std::mt19937{std::random_device{}()});

  std::cout << "Contents of the list: "s;
  for (auto n_ : lst) {
    std::cout << n_ << ' ';
  }
  std::cout << std::endl;

  std::cout << "Contents of the list, shuffled: "s;
  for (auto i_ : vec) {
    std::cout << *i_ << ' ';
  }
  std::cout << std::endl;

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_accumulate()
 */
void fn_accumulate(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  std::vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, };

  int sum = std::accumulate(vec.begin(), vec.end(), 0);

  int product = std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());

  auto dash_fold = [](std::string a_, int b_) {
    return std::move(a_) + '-' + std::to_string(b_);
  };

  std::string as = std::accumulate(std::next(vec.begin()), vec.end(),
                                   std::to_string(vec[0]), // start with first element
                                   dash_fold);

  // Right fold using reverse iterators
  std::string rs = std::accumulate(std::next(vec.rbegin()), vec.rend(),
                                   std::to_string(vec.back()), // start with last element
                                   dash_fold);

  std::cout << "sum: "s << sum << '\n'
            << "product: "s << product << '\n'
            << "dash-separated string: "s << as << '\n'
            << "dash-separated string (right-folded): "s << rs << '\n';

  auto avg = static_cast<double>(std::accumulate(vec.begin(), vec.end(), 0)) / vec.size();
  std::cout << "average: "s << avg << std::endl;
  for (auto e_ = vec.begin(); e_ != vec.end();) {
    double cv = *e_;
    if (cv < avg) {
      vec.erase(e_);
    }
    else {
      ++e_;
    }
  }
#ifdef AS_USE_COPY_
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';
#else
  std::for_each(vec.cbegin(), vec.cend(), [](auto const n) { std::cout << n << ' '; });
  std::cout << '\n';
#endif

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_reduce()
 */
void fn_reduce(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  const std::vector<double> vec(10'000'007, 0.5);

  {
    const auto t1 = std::chrono::high_resolution_clock::now();

    const double result = std::accumulate(vec.cbegin(), vec.cend(), 0.0);

    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << std::fixed << "std::accumulate result "s << result
              << " took "s << ms.count() << " ms"s
              << std::endl;
  }

#if defined(_LIBCPP_HAS_PARALLEL_ALGORITHMS)
  {
    const auto t1 = std::chrono::high_resolution_clock::now();

    const double result = std::reduce(std::execution::par, v.cbegin(), v.cend());

    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "std::reduce result "s
              << result << " took "s << ms.count() << " ms"s
              << std::endl;
  }
#else
  {
    const auto t1 = std::chrono::high_resolution_clock::now();

    const double result = std::reduce(vec.cbegin(), vec.cend());

    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "std::reduce result "s
              << result << " took "s << ms.count() << " ms"s
              << std::endl;
  }
#endif

  {
    const auto t1 = std::chrono::high_resolution_clock::now();

    const double result = std::reduce(vec.cbegin(), vec.cend(), 777.7);

    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "std::reduce result "s
              << result << " took "s << ms.count() << " ms"s
              << std::endl;
  }

  {
    const auto t1 = std::chrono::high_resolution_clock::now();

    const double result = std::reduce(vec.cbegin(), vec.cend(), 0, [](auto n1, auto n2) {
      return n1 + n2 * 11.5;
    });

    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "std::reduce result "s
              << result << " took "s << ms.count() << " ms"s
              << std::endl;
  }

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_transform_reduce()
 */
void fn_transform_reduce(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

#define AS_LAMBDA_DEBUG_ 1

  std::vector<double> xvalues(10007, 1.0);
  std::vector<double> yvalues(10007, 1.0);

#if defined(_LIBCPP_HAS_PARALLEL_ALGORITHMS)
  {
    double result = std::transform_reduce(
      std::execution::par,
      xvalues.begin(), xvalues.end(),
      yvalues.begin(), 0.0
    );
    std::cout << result << '\n';
  }
#else
  {
    
    double result = std::transform_reduce(xvalues.begin(), xvalues.end(), yvalues.begin(), 0.0);
    std::cout << "std::transform_reduce result "s
              << result << '\n';
  }
#endif

  auto cx = 0;
  auto constexpr cx_max = 25;
  auto pl = [& cx](auto const n) {
    std::cout << std::setw(4) << n << ((++cx % cx_max == 0) ? "\n"s : ""s);
  };

  {
    std::list<int> lstx { 0, 1, 2, 3, 4, };
    std::list<int> lsty { 5, 4, 2, 3, 1, };

    std::cout << "std::list lstx:"s << '\n';
    auto sumx = std::accumulate(lstx.begin(), lstx.end(), 0);
    cx = 0;
    std::for_each(lstx.begin(), lstx.end(), pl);
    std::cout << '\n' << "std::accumulate sum "s << sumx << '\n';

    std::cout << "std::list lsty:"s << '\n';
    auto sumy = std::accumulate(lsty.begin(), lsty.end(), 0);
    cx = 0;
    std::for_each(lsty.begin(), lsty.end(), pl);
    std::cout << '\n' << "std::accumulate sum "s << sumy << '\n';

    auto result = std::transform_reduce(lstx.begin(), lstx.end(), lsty.begin(), 0);
    std::cout << "std::transform_reduce lstx lsty "s << result << '\n';

    auto prod = std::inner_product(lstx.begin(), lstx.end(), lsty.begin(), 0);
    std::cout << "std::inner_product lstx lsty "s << prod << '\n';
  }

  std::cout << '\n';

  {
    std::vector<int> vecx { 0, 1, 2, 3, 4, };
    std::vector<int> vecy { 5, 4, 2, 3, 1, };

    std::cout << "std::vector vecx & vecy: "s << '\n';
    cx = 0;
    std::for_each(vecx.begin(), vecx.end(), pl);
    std::cout << '\n';
    cx = 0;
    std::for_each(vecy.begin(), vecy.end(), pl);
    std::cout << '\n';

    auto vip = std::inner_product(vecx.begin(), vecx.end(), vecy.begin(), 0);
    std::cout << "Inner product of vecx and vecy: "s << vip << "\n"s;
    auto vtr = std::transform_reduce(vecx.begin(), vecx.end(), vecy.begin(), 0);
    std::cout << "Transform-reduce of vecx and vecy: "s << vtr << "\n\n"s;

    std::list<int> lstx { 0, 1, 2, 3, 4, };
    std::list<int> lsty { 5, 4, 2, 3, 1, };

    std::cout << "std::list lstx & lsty: "s << '\n';
    cx = 0;
    std::for_each(lstx.begin(), lstx.end(), pl);
    std::cout << '\n';
    cx = 0;
    std::for_each(lsty.begin(), lsty.end(), pl);
    std::cout << '\n';

    auto lip = std::inner_product(lstx.begin(), lstx.end(), lsty.begin(), 0);
    std::cout << "Inner product of lstx and lsty: "s << lip << "\n"s;
    auto ltr = std::transform_reduce(lstx.begin(), lstx.end(), lsty.begin(), 0);
    std::cout << "Transform-reduce of lstx and lsty: "s << ltr << "\n\n"s;

    //  --------------------------------------------------------------------------------
    //  Sum values from 1 - 100, std::list
    //  transform_reduce using BinaryOp (emulates std::plus), UnaryOp (nop)
    std::list<int> l1(100, 1);
    std::list<int> l2(100);
    size_t cc = 0;
    size_t constexpr cc_max = 25;
    std::for_each(l1.begin(), l1.end(), [& cc](auto n_) {
      std::cout << std::setw(2) << n_ << (++cc % cc_max == 0 ? "\n"s : ""s);
    });
    std::cout << std::endl;

    std::generate_n(l2.begin(), l2.size(), [n(0)]() mutable { return ++n; });
    std::for_each(l2.begin(), l2.end(), [& cc](auto n_) {
      std::cout << std::setw(4) << n_ << (++cc % cc_max == 0 ? "\n"s : ""s);
    });
    std::cout << std::endl;
    auto trlr =
    std::transform_reduce(l2.begin(), l2.end(), 0,
                          /*std::plus<>(),*/
                          [](auto n1, auto n2) {
      auto rr = n1 + n2;
#ifdef AS_LAMBDA_DEBUG_
      std::cout << "BinaryOp: "s << __func__
                << std::setw(5) << n1
                << std::setw(5) << n2
                << std::setw(5) << rr
                << '\n';
#endif
      return rr;
    },
                          [](auto n_) {
      auto rn = n_;
#ifdef AS_LAMBDA_DEBUG_
      std::cout << "UnaryOp   "s << __func__
                << std::setw(5) << rn
                << '\n';
#endif
      return rn;
    });
    std::cout << std::setw(20) << "std::list "s << trlr << std::endl;
//    auto l1sum = std::reduce(l1.begin(), l1.end(), 0);
//    std::cout << l1sum << std::endl;

    //  --------------------------------------------------------------------------------
    //  Sum values from 1 - 100, std::vector
    //  transform_reduce using BinaryOp1 (emulates std::plus), BinaryOp2 (emulates std::multiplies)
    std::vector<int> v1(100, 1);
    std::vector<int> v2(100);
    std::iota(v2.begin(), v2.end(), 1);
    auto trvr =
    std::transform_reduce(v2.begin(), v2.end(), v1.begin(), 0,
                          [](auto n1, auto n2) {
      auto rr = n1 + n2;
#ifdef AS_LAMBDA_DEBUG_
      std::cout << "BinaryOp1: " << __func__
                << std::setw(5) << n1
                << std::setw(5) << n2
                << std::setw(5) << rr
                << '\n';
#endif
      return rr;
    },
                          [](auto n1, auto n2) {
      auto rr = n1 * n2;
#ifdef AS_LAMBDA_DEBUG_
      std::cout << "BinaryOp2: " << __func__
                << std::setw(5) << n1
                << std::setw(5) << n2
                << std::setw(5) << rr
                << '\n';
#endif
      return rr;
    });
    std::cout << std::setw(20) << "std::vector "s << trvr << std::endl;

    //  --------------------------------------------------------------------------------
    //  Sum values from 1 - 100, std::array
    //  transform_reduce using std::plus, std::multiplies
    std::array<int, 100> a1;
    std::array<int, 100> a2;
    a1.fill(1);
    std::iota(a2.begin(), a2.end(), 1);
    auto trar =
    std::transform_reduce(a2.begin(), a2.end(), a1.begin(), 0, std::plus<>(), std::multiplies<>());
    std::cout << std::setw(20) << "std::array "s << trar << std::endl;

    //  --------------------------------------------------------------------------------
    //  Sum values from 1 - 100, std::forward_list
    //  transform_reduce using defaults
    std::forward_list<int> f1(100, 1);
    std::forward_list<int> f2(100);
    std::generate(f2.begin(), f2.end(), []() { static int n(0); return ++n; });
    auto trfr =
    std::transform_reduce(a2.begin(), a2.end(), a1.begin(), 0);
    std::cout << std::setw(20) << "std::forward_list "s << trfr << std::endl;

    //  --------------------------------------------------------------------------------
    std::deque<int> d1(100, 1);
    std::deque<int> d2(100);
    std::iota(d2.begin(), d2.end(), 1);
    auto trdr =
    std::transform_reduce(d2.begin(), d2.end(), d1.begin(), 0);
    std::cout << std::setw(20) << "std::deque "s << trdr << std::endl;

    //  --------------------------------------------------------------------------------
    std::deque<int> ds(100);
    std::iota(ds.begin(), ds.end(), 1);
    std::set<int> s1;
    std::for_each(ds.cbegin(), ds.cend(), [&s1](auto const sn){ s1.insert(sn); });
    auto trsr =
    std::transform_reduce(s1.begin(), s1.end(), d1.cbegin(), 0);
    std::cout << std::setw(20) << "std::set "s << trsr << std::endl;

    cx = 0;
    std::for_each(s1.cbegin(), s1.cend(), pl);
    std::cout << std::endl;

    //  --------------------------------------------------------------------------------
#if __cplusplus >= 201707 /* C++ 2a annd greater */
    int data1[100];
    int data2[100];
    std::span<int> sdata1 { data1 };
    std::span<int> sdata2 { data2 };
    std::iota(sdata1.begin(), sdata1.end(), 1); //  1 - 100 for summing
    std::fill(sdata2.begin(), sdata2.end(), 1); //  just 1s for multiplier

    auto trsp =
    std::transform_reduce(sdata1.begin(), sdata1.end(), sdata2.begin(), 0);

    std::cout << std::setw(20) << "std::span "s << trsp << std::endl;
#endif  /* __cplusplus >= 201707 */
  }
  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_inner_product()
 */
void fn_inner_product(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  std::vector<int> veca { 0, 1, 2, 3, 4, };
  std::vector<int> vecb { 5, 4, 2, 3, 1, };

  auto pl = [](auto const n) { std::cout << n << ' '; };

  std::cout << "std::vector veca & vecb: "s << '\n';
  std::for_each(veca.begin(), veca.end(), pl);
  std::cout << '\n';
  std::for_each(vecb.begin(), vecb.end(), pl);
  std::cout << '\n';

  int r1 = std::inner_product(veca.begin(), veca.end(), vecb.begin(), 0);
  std::cout << "Inner product of a and b: " << r1 << '\n';

  int r2 = std::inner_product(veca.begin(), veca.end(), vecb.begin(), 0,
                              std::plus<>(), std::equal_to<>());
  std::cout << "Number of pairwise matches between a and b: " <<  r2 << '\n';

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_adjacent_difference()
 */
void fn_adjacent_difference(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  // Default implementation - the difference b/w two adjacent items
  std::vector vec { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, };

  std::for_each(vec.begin(), vec.end(), [](auto const n_) { std::cout << std::setw(2) << n_ << ' '; });
  std::cout << '\n';

  std::adjacent_difference(vec.begin(), vec.end(), vec.begin());

  for (auto n_ : vec) {
    std::cout << std::setw(2) << n_ << ' ';
  }
  std::cout << '\n';

  // TODO: Fibonacci Rule is xᵤ = xᵤ₋₁ + xᵤ₋₂
  std::cout << "\nFibonacci series, xᵤ = xᵤ₋₁ + xᵤ₋₂ [first 93 terms]\n"s;
  std::array<uint64_t, 93> ary { 1 };

  std::adjacent_difference(ary.begin(),
                           std::prev(ary.end()), std::next(ary.begin()), std::plus<> {});
  std::cout << '\n';
  size_t col(0);
  size_t const col_max(3);
  std::for_each(ary.begin(), ary.end(), [& col](auto const n_) {
    std::cout << std::setw(20) << n_ << (++col % col_max == 0 ? '\n' : ' ');
  });
//  std::copy(begin(ary), end(ary), std::ostream_iterator<int> {std::cout, " "});
  std::cout << '\n';

  uint64_t constexpr fibonacci[] {
  /*  0 */                          1ULL,                          1ULL,                          2ULL,
  /*  3 */                          3ULL,                          5ULL,                          8ULL,
  /*  6 */                         13ULL,                         21ULL,                         34ULL,
  /*  9 */                         55ULL,                         89ULL,                        144ULL,
  /* 12 */                        233ULL,                        377ULL,                        610ULL,
  /* 15 */                        987ULL,                      1'597ULL,                      2'584ULL,
  /* 18 */                      4'181ULL,                      6'765ULL,                     10'946ULL,
  /* 21 */                     17'711ULL,                     28'657ULL,                     46'368ULL,
  /* 24 */                     75'025ULL,                    121'393ULL,                    196'418ULL,
  /* 27 */                    317'811ULL,                    514'229ULL,                    832'040ULL,
  /* 30 */                  1'346'269ULL,                  2'178'309ULL,                  3'524'578ULL,
  /* 33 */                  5'702'887ULL,                  9'227'465ULL,                 14'930'352ULL,
  /* 36 */                 24'157'817ULL,                 39'088'169ULL,                 63'245'986ULL,
  /* 39 */                102'334'155ULL,                165'580'141ULL,                267'914'296ULL,
  /* 42 */                433'494'437ULL,                701'408'733ULL,              1'134'903'170ULL,
  /* 45 */              1'836'311'903ULL,              2'971'215'073ULL,              4'807'526'976ULL,
  /* 48 */              7'778'742'049ULL,             12'586'269'025ULL,             20'365'011'074ULL,
  /* 51 */             32'951'280'099ULL,             53'316'291'173ULL,             86'267'571'272ULL,
  /* 54 */            139'583'862'445ULL,            225'851'433'717ULL,            365'435'296'162ULL,
  /* 57 */            591'286'729'879ULL,            956'722'026'041ULL,          1'548'008'755'920ULL,
  /* 60 */          2'504'730'781'961ULL,          4'052'739'537'881ULL,          6'557'470'319'842ULL,
  /* 63 */         10'610'209'857'723ULL,         17'167'680'177'565ULL,         27'777'890'035'288ULL,
  /* 66 */         44'945'570'212'853ULL,         72'723'460'248'141ULL,        117'669'030'460'994ULL,
  /* 69 */        190'392'490'709'135ULL,        308'061'521'170'129ULL,        498'454'011'879'264ULL,
  /* 72 */        806'515'533'049'393ULL,      1'304'969'544'928'657ULL,      2'111'485'077'978'050ULL,
  /* 75 */      3'416'454'622'906'707ULL,      5'527'939'700'884'757ULL,      8'944'394'323'791'464ULL,
  /* 78 */     14'472'334'024'676'221ULL,     23'416'728'348'467'685ULL,     37'889'062'373'143'906ULL,
  /* 81 */     61'305'790'721'611'591ULL,     99'194'853'094'755'497ULL,    160'500'643'816'367'088ULL,
  /* 84 */    259'695'496'911'122'585ULL,    420'196'140'727'489'673ULL,    679'891'637'638'612'258ULL,
  /* 87 */  1'100'087'778'366'101'931ULL,  1'779'979'416'004'714'189ULL,  2'880'067'194'370'816'120ULL,
  /* 90 */  4'660'046'610'375'530'309ULL,  7'540'113'804'746'346'429ULL, 12'200'160'415'121'876'738ULL,
  };
  size_t constexpr fibonacci_l = sizeof(fibonacci) / sizeof(*fibonacci);
  col = 0;
  std::for_each_n(std::begin(fibonacci), fibonacci_l, [&col](auto f_) {
    std::cout << std::setw(20) << f_ << (++col % col_max == 0 ? '\n' : ' ');
  });
  std::cout << '\n'
            << std::setw(40 + 2) << "Max 64-bit unsigned integer ULLONG_MAX: "s
            << std::setw(20) << ULLONG_MAX << '\n';

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_partial_sum()
 */
void fn_partial_sum(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  std::vector<int> vec = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, }; // or std::vector<int>v(10, 2);

  auto pl = [](auto n_) { std::cout << n_ << ' '; };

  std::for_each(vec.begin(), vec.end(), pl);
  std::cout << '\n';

  std::cout << "The first "s << vec.size() << " even numbers are: "s;
  std::partial_sum(vec.begin(), vec.end(),
                   std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  std::for_each(vec.begin(), vec.end(), pl);
  std::cout << '\n';

  std::partial_sum(vec.begin(), vec.end(), vec.begin(), std::multiplies<int>());
  std::cout << "The first "s << vec.size() << " powers of 2 are: "s;
//  for (auto n : vec) {
//    std::cout << n << ' ';
//  }
  std::for_each(vec.begin(), vec.end(), pl);
  std::cout << '\n';

  std::cout << '\n';
  std::cout << "Factorials (using std::partial_sum):\n"s;
  size_t const max_factor(20);  //  20! is the largest factorial that will fit in a 64-bit integer.
  std::vector<uint64_t> vidx(max_factor, 1);
  std::vector<uint64_t> vfact(max_factor);
  std::for_each(vidx.begin(), vidx.end(), [](auto n_) { std::cout << std::setw(2) << n_ << ' '; });
  std::cout << '\n';
  std::partial_sum(vidx.begin(), vidx.end(), vfact.begin());
  std::for_each(vfact.begin(), vfact.end(), [](auto n_) { std::cout << std::setw(2) << n_ << ' '; });
  std::cout << '\n';
  std::partial_sum(vfact.begin(), vfact.end(), vfact.begin(), std::multiplies<uint64_t>());
  std::for_each(vfact.begin(), vfact.end(), [](auto n_) {
    std::cout << std::setw(20) << n_ << '\n';
  });

  //  From GMP.Factorial.Select
  uint64_t constexpr factorials[] = {
    /* 0!*/                         0ULL, /* TODO: Added to make picking factorials intuitive */
    /* 1!*/                         1ULL,
    /* 2!*/                         2ULL,
    /* 3!*/                         6ULL,
    /* 4!*/                        24ULL,
    /* 5!*/                       120ULL,
    /* 6!*/                       720ULL,
    /* 7!*/                     5'040ULL,
    /* 8!*/                    40'320ULL,
    /* 9!*/                   362'880ULL,
    /*10!*/                 3'628'800ULL,
    /*11!*/                39'916'800ULL,
    /*12!*/               479'001'600ULL, /* TODO: max factorial for 32-bit integers */
    /*13!*/             6'227'020'800ULL,
    /*14!*/            87'178'291'200ULL,
    /*15!*/         1'307'674'368'000ULL,
    /*16!*/        20'922'789'888'000ULL,
    /*17!*/       355'687'428'096'000ULL,
    /*18!*/     6'402'373'705'728'000ULL,
    /*19!*/   121'645'100'408'832'000ULL,
    /*20!*/ 2'432'902'008'176'640'000ULL, /* TODO: max factorial for 64-bit integers */
    };
  size_t constexpr factorials_c = sizeof(factorials) / sizeof(*factorials);
  std::for_each_n(std::begin(factorials), factorials_c, [](auto n_) {
    if (n_ != 0) {
      std::cout << std::setw(20) << n_ << '\n';
    }
  });

  std::cout << '\n';

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_exclusive_scan_inclusive_scan()
 */
void fn_exclusive_scan_inclusive_scan(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  {
    std::vector<int> data { 3, 1, 4, 1, 5, 9, 2, 6, };

    std::cout << "exclusive sum: "s;
    std::exclusive_scan(data.begin(), data.end(),
                        std::ostream_iterator<int>(std::cout, " "),
                        0);

    std::cout << "\ninclusive sum: "s;
    std::inclusive_scan(data.begin(), data.end(),
                        std::ostream_iterator<int>(std::cout, " "));


    std::cout << "\n\nexclusive product: "s;
    std::exclusive_scan(data.begin(), data.end(),
                        std::ostream_iterator<int>(std::cout, " "),
                        1, std::multiplies<>{});

    std::cout << "\ninclusive product: "s;
    std::inclusive_scan(data.begin(), data.end(),
                        std::ostream_iterator<int>(std::cout, " "),
                        std::multiplies<>{});
    std::cout << '\n';
  }

  std::cout << std::endl;

  {
    size_t constexpr pad(20);
    size_t constexpr pw(4);
    auto pf = [](auto n_) {
      std::cout << std::setw(pw) << n_ << ' ';
    };
    std::vector<int> i_data { 3, 1, 4, 1, 5, 9, 2, 6, };
    std::vector<int> o_data(i_data.size());

    std::cout << std::setw(pad) << "input data: "s;
    std::for_each(i_data.begin(), i_data.end(), pf);
    std::cout << '\n' << '\n';

    std::cout << std::setw(pad) << "exclusive sum: "s;
    std::exclusive_scan(i_data.begin(), i_data.end(),
                        o_data.begin(),
                        0);
    std::for_each(o_data.begin(), o_data.end(), pf);
    std::cout << '\n';

    std::cout << std::setw(pad) << "inclusive sum: "s;
    std::inclusive_scan(i_data.begin(), i_data.end(),
                        o_data.begin());
    std::for_each(o_data.begin(), o_data.end(), [](auto n_) {
      std::cout << std::setw(pw) << n_ << ' ';
    });
    std::cout << '\n' << '\n';

    std::cout << std::setw(pad) << "exclusive product: "s;
    std::exclusive_scan(i_data.begin(), i_data.end(),
                        o_data.begin(),
                        1,
                        std::multiplies<>{});
    std::for_each(o_data.begin(), o_data.end(), pf);
    std::cout << '\n';

    std::cout << std::setw(pad) << "inclusive product: "s;
    std::inclusive_scan(i_data.begin(), i_data.end(),
                        o_data.begin(),
                        std::multiplies<>{});
    std::for_each(o_data.begin(), o_data.end(), pf);
    std::cout << '\n';
  }

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_transform_exclusive_scan_transform_inclusive_scan()
 */
void fn_transform_exclusive_scan_transform_inclusive_scan(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  {
    std::vector<int> data { 3, 1, 4, 1, 5, 9, 2, 6, };

    auto times_10 = [](int x) { return x * 10; };

    std::cout << "10 times exclusive sum: ";
    std::transform_exclusive_scan(data.begin(), data.end(),
                                  std::ostream_iterator<int>(std::cout, " "),
                                  0,
                                  std::plus<int>{}, times_10);
    std::cout << "\n10 times inclusive sum: ";
    std::transform_inclusive_scan(data.begin(), data.end(),
                                  std::ostream_iterator<int>(std::cout, " "),
                                  std::plus<int>{}, times_10);
    std::cout << '\n' << '\n';
  }

  std::cout << std::endl;

  {
    size_t constexpr pad(26);
    size_t constexpr pw(4);
    auto pf = [](auto n_) {
      std::cout << std::setw(pw) << n_ << ' ';
    };
    std::vector<int> i_data { 3, 1, 4, 1, 5, 9, 2, 6, };
    std::vector<int> o_data(i_data.size());

    auto times_10 = [](int x) {
      return x * 10;
    };

    std::cout << std::setw(pad) << "input data: "s;
    std::for_each(i_data.begin(), i_data.end(), pf);
    std::cout << '\n' << '\n';

    std::cout << std::setw(pad) << "10 times exclusive sum: ";
    std::transform_exclusive_scan(i_data.begin(), i_data.end(),
                                  o_data.begin(),
                                  0, std::plus<int>{},
                                  times_10);
    std::for_each(o_data.begin(), o_data.end(), pf);
    std::cout << '\n';
    std::cout << std::setw(pad) << "10 times inclusive sum: ";
    std::transform_inclusive_scan(i_data.begin(), i_data.end(),
                                  o_data.begin(),
                                  std::plus<int>{},
                                  times_10);
    std::for_each(o_data.begin(), o_data.end(), pf);
    std::cout << '\n' << '\n';
  }

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_gcd()
 */
void fn_gcd(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  std::vector<int32_t>  vals(30);
  std::iota(vals.begin(), vals.end(), 21);
  int32_t constexpr cv(3);
  size_t constexpr sw(2);
  std::cout << "greatest common divisors:"s << '\n';
  std::for_each(vals.begin(), vals.end(), [](auto v_) {
    int32_t vgcd = std::gcd(cv, v_);
    std::cout << "gcd of "s << std::setw(sw) << cv
              << " & "s     << std::setw(sw) << v_
              << " is "s    << std::setw(sw) << vgcd
              << '\n';
  });

  std::cout << std::endl;

  return;;
}

/*
 *  MARK: fn_lcm()
 */
void fn_lcm(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  std::vector<int32_t>  vals(30);
  std::iota(vals.begin(), vals.end(), 21);
  int32_t constexpr cv(3);
  size_t constexpr sw(2);
  std::cout << "least common multiples:"s << '\n';
  std::for_each(vals.begin(), vals.end(), [](auto v_) {
    int32_t vlcm = std::lcm(cv, v_);
    std::cout << "lcm of "s << std::setw(sw) << cv
              << " & "s     << std::setw(sw) << v_
              << " is "s    << std::setw(sw) << vlcm
              << '\n';
  });

  std::cout << std::endl;

  return;
}

/*
 *  MARK: fn_midpoint()
 */
void fn_midpoint(void) {
  std::cout << "Function: "s << __func__ << std::endl;
  std::cout
    << "--------------------------------------------------------------------------------"s
    << '\n'
    << std::endl;

  {
    std::uint32_t max_a = std::numeric_limits<std::uint32_t>::max();
    std::uint32_t max_b = std::numeric_limits<std::uint32_t>::max() - 2;

    std::cout << "max_a: " << max_a << '\n'
              << "max_b: " << max_b << '\n'
              << "Incorrect (overflow and wrapping): "s << (max_a + max_b) / 2 << '\n'
              << "Correct: "s << std::midpoint(max_a, max_b) << "\n\n"s;

    auto on_pointers = [](int i_, int j_) {
      char const * text = "0123456789";
      char const * p_ = text + i_;
      char const * q_ = text + j_;
      std::cout << "std::midpoint('"s << *p_ << "', '"s << *q_ << "'): '"s
                << *std::midpoint(p_, q_) << "'\n"s;
    };

    on_pointers(2, 4);
    on_pointers(2, 5);
    on_pointers(5, 2);
    on_pointers(2, 6);
  }
  
  std::cout << std::endl;

  return;
}
