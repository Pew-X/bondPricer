#include "generaltermbond.hpp"

using namespace BondLibrary;

GeneralTermBond::GeneralTermBond(double face_value, double coupon, const Date maturity_date,
 const Date issue_date, const CashFlowsPy& cashflows, const Date settlement_date, 
 YieldCurve& yield_curve, const DayCountConvention daycount_convention)
  : BaseBond(face_value, coupon, maturity_date, issue_date, cashflows, settlement_date, daycount_convention)
  , yield_curve_(yield_curve)
{}

double GeneralTermBond::cleanPrice(const Date date) const {
    if (isExpired()) return 0.0;
    return valueBasedOnYieldCurve(0, date);
}

double GeneralTermBond::dirtyPrice(const Date date) const {
    if (isExpired()) return 0.0;
    return round((valueBasedOnYieldCurve(0, date) + accruedAmount(date)) * 100.0) / 100.0;
}
/*
double GeneralTermBond::dirtyPrice(const double market_price, const Date date) const {
    if (isExpired()) return 0.0;
    return market_price + accruedAmount(date);
}
*/

double GeneralTermBond::valueBasedOnYieldCurve(const double, Date date) const {
    double npv = 0.0;
    size_t t = 1;
    for (size_t i = 0; i < cashflows_.size(); ++i) {
        if (cashflows_[i].due_date < date) continue;
        npv += discountFactorFromYield(performLinearInterpolation(
            getYearFraction(cashflows_[i].due_date) + yearsAccrued(cashflows_[i].due_date)
        ), t) * cashflows_[i].cashflow;
        ++t;
    }
    return round((npv * 100.0)) / 100.0;
}

double GeneralTermBond::duration(const double, Date date) const {
    double s = 0.0, d1 = 0.0;
    double dfactor = 0.0;
    int t = 1;
    for (size_t i = 0; i < cashflows_.size(); ++i) {
        if (cashflows_[i].due_date < date) continue;
        dfactor = discountFactorFromYield(
            performLinearInterpolation(getYearFraction(cashflows_[i].due_date) + yearsAccrued(cashflows_[i].due_date)
        ), t);
        s += cashflows_[i].cashflow * dfactor;
        d1 += t * cashflows_[i].cashflow * dfactor;
        ++t;
    }
    return d1 / s;
}

double GeneralTermBond::getDuration(const Date date) const {
    return round(duration(0, date) * 100.0) / 100.0;
}

double GeneralTermBond::discountFactorFromYield(const double rate, const double time) const {
    return exp(-rate * time);
}

double GeneralTermBond::performLinearInterpolation(const double time) const {
    const auto& yield_curve = yield_curve_.getYieldCurve(); // Yields are in increasing time to maturity order
    if (yield_curve.size() < 1) return 0.0;
    auto t_min = yield_curve[0].maturity;
    const auto& curve_max = yield_curve[yield_curve.size() - 1];
    if (time <= t_min) {
        return yield_curve[0].yield;
    }    
    else if (time >= curve_max.maturity) {
        return curve_max.yield;
    }
    size_t t = 1;
    while (t < yield_curve.size() && time > yield_curve[t].maturity)
        ++t;
    double lambda = (yield_curve[t].maturity - time) / (yield_curve[t].maturity - yield_curve[t - 1].maturity);
    return yield_curve[t - 1].yield * lambda + yield_curve[t].yield * (1.0 - lambda);
}

double GeneralTermBond::getYearFraction(const Date& date) const {
    double frac = 0.0;
    for (int i = 0; i < date.month; ++i)
        frac += BaseBond::month_days_[i];
    return (frac + static_cast<double>(date.day - 1)) / 365.0;
}

int GeneralTermBond::yearsAccrued(const Date& date) const {
    return abs(date.year - cashflows_[0].due_date.year + 1); // year 0 counts as 'year 1'
}
