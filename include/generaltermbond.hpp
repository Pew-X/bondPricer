#ifndef GENERAL_TERM_BOND_HPP
#define GENERAL_TERM_BOND_HPP

#include <vector>
#include <cmath>

#include "basebond.hpp"
#include "yieldcurve.hpp"

namespace BondLibrary {
class GeneralTermBond : public BaseBond {
public:
    GeneralTermBond(
        double face_value,
        double coupon,
        const Date maturity_date,
        const Date issue_date,
        const CashFlowsPy& cashflows,
        const Date settlement_date,
        YieldCurve& yield_curve,
        const DayCountConvention
    );
    double cleanPrice(const Date date) const;
    double dirtyPrice(const Date date) const;
    //double dirtyPrice(const double market_price, const Date date) const;
    double getDuration(const Date date) const;
    double duration(const double rate, const Date date) const override;
    void setYieldCurve(YieldCurve& yc) const {yield_curve_ = yc;}
    YieldCurve& getYieldCurve() const {return yield_curve_;}
private:
    int yearsAccrued(const Date& date) const;
    double getYearFraction(const Date& date) const;
    double discountFactorFromYield(const double rate, const double time) const;
    double valueBasedOnYieldCurve(const double rate, Date date) const;
    double performLinearInterpolation(const double time) const;
    YieldCurve& yield_curve_;
};
}

#endif
