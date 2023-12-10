

This is a straightforward library designed for pricing Corporate and Treasury bonds. It offers a Boost Python interface for seamless integration with Python.

Key Features:
- Generation of Yield Curve
- Interpolation of Yield and Price
- Calculation of Dirty and Clean pricing
- Yield to Maturity (YTM) using Safe-Newton approximations
- Support for both Flat and General Term Structure
- Day-count conventions including 30/360, 30/365, actual/360, actual/365, and actual/actual

Usage:
The library is user-friendly. To illustrate, pricing a bond with a 40-year maturity and a 2% annual coupon using a UK gilt yield curve from the Bank of England can be achieved with the following code:

```python
from BondPricing import *
import pandas as pd

xls = pd.ExcelFile('spotcurve/GLC Nominal daily data current month.xlsx')
df = pd.read_excel(xls, '4. spot curve')

curve = YieldCurve([
    YieldCurvePoint(
        maturity=df.iat[2, x],
        bond_yield=df.iat[4, x]
    ) for x in range(1, len(df.columns))
])

gt_bond = GeneralTermBond(
    face_value=1000,
    coupon=20,
    cashflows=[
        CashFlow(20, Date('01/01/202{}'.format(x))) for x in range(2, 17)
    ],
    maturity_date=Date('01/01/2036'),
    issue_date=Date('01/01/2021'),
    yield_curve=curve,
    dc_convention=DayCountConvention.YearActualMonthActual
)

clean_price = gt_bond.cleanPrice(Date('01/01/2021'))
print(clean_price)
print(gt_bond.yieldToMaturity(clean_price, Date('01/01/2021')))
```

Building The Bond Pricing Library:
The library follows the standard CMake build pattern. From the project root directory:

```bash
mkdir build
cd build
cmake ..
make
```

This will create the target `BondPricing.so`. You can import `BondPricing` in any Python3 script as a module and utilize the library.

Note: The library requires Boost Configuration of Python3. Building Boost with Python3 from source is possible by editing `user-config.jam` and following the provided instructions.

Dependencies:
- Python 3: [https://www.python.org/](https://www.python.org/)
- Boost: [https://www.boost.org/](https://www.boost.org/)
- Pytest (tests): [https://docs.pytest.org/en/6.2.x/](https://docs.pytest.org/en/6.2.x/)
- Cmake (build system): [https://cmake.org/](https://cmake.org/)
