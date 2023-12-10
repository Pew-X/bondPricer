# Bond Pricing

A simple Corporate and Treasury bond pricing library. The library provides an interface via Boost Python for usage in Python.

## Features

* Yield Curve Generation
* Yield and Price interpolation
* Dirty and Clean pricing
* Yield to Maturity (YTM) Safe-Newton approximations
* Flat and General Term Structure
* 30/360, 30/365, actual/360, actual/365 and actual/actual day-count conventions

## Usage

The library is simple to use. For example, to price a bond with a 40 year maturity and 2% annual coupon using a UK gilt (government bond) yield curve provided by the Bank of England, the library is used as below:

```python
from BondPricing import *
import pandas as pd

xls = pd.ExcelFile('spotcurve/GLC Nominal daily data current month.xlsx')
df = pd.read_excel(xls, '4. spot curve')
    
curve = YieldCurve([
    YieldCurvePoint(
        maturity = df.iat[2, x],
        bond_yield = df.iat[4, x]
    ) for x in range(1, len(df.columns))
])

gt_bond = GeneralTermBond(
    face_value = 1000,
    coupon = 20,
    cashflows = [
        CashFlow(20, Date('01/01/202{}'.format(x))) for x in range(2, 17)
    ],
    maturity_date = Date('01/01/2036'),
    issue_date = Date('01/01/2021'),
    yield_curve = curve,
    dc_convention = DayCountConvention.YearActualMonthActual
)

clean_price = gt_bond.cleanPrice(Date('01/01/2021'))
print(clean_price)
print(gt_bond.yieldToMaturity(clean_price, Date('01/01/2021')))
```

## Building The Bond Pricing Library

The library is built with the usual CMake build pattern, from the project root directory:

```
mkdir build
cd build
cmake ..
make
```

This will build the target BondPricing.so. You can 'import BondPricing' with any Python3 script just like any other module and use the library.

Note that this library uses Python3 in its linkage with Boost::Python, and a Boost Configuration of Python3 is required (by default, Boost builds with Python2).

To build Boost with Python3 from source, first edit user-config.jam in tools/build/src to contain:

```
using python : 3.8 : /usr/bin/python3 : /usr/include/python3.8 : /usr/lib ;
```
Then build Boost:
```
./bootstrap.sh --prefix=INSTALL_DIR
./b2 --with-python cxxflags="-std=c++11 -fPIC" variant=release stage
./b2 --with-python cxxflags="-std=c++11 -fPIC" variant=release install
```

## Dependencies

* Python 3: https://www.python.org/
* Boost: https://www.boost.org/
* Pytest (tests): https://docs.pytest.org/en/6.2.x/
* Cmake (build system): https://cmake.org/
