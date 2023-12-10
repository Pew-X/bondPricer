#include "flattermbond.hpp"
#include <iostream>

using namespace BondLibrary;

FlatTermBond::FlatTermBond(double face_value, double coupon, const Date maturity_date,
 const Date issue_date, const CashFlowsPy& cashflows, Date settlement_date,
 const DayCountConvention daycount_convention)
    : BaseBond(face_value, coupon, maturity_date, issue_date, cashflows, settlement_date, daycount_convention)
{}

double FlatTermBond::cleanPrice(const double rate, const Date date) const {
    return notionalPresentValue(rate, date);
}

double FlatTermBond::dirtyPrice(const double rate, const Date date) const {
    return notionalPresentValue(rate, date) + accruedAmount(date);
}

double FlatTermBond::dirtyPriceFromCleanPrice(const double market_price, const Date date) const {
    return market_price + accruedAmount(date);
}

double FlatTermBond::duration(const double rate, const Date date) const {
    size_t t = 1;
    double duration = 0.0;
    for (const auto& cashflow : cashflows_) {
        if (cashflow.due_date < date) continue;
        duration += (t * cashflow.cashflow) / pow((1 + rate), t);
        ++t;
    }
    return round((duration / notionalPresentValue(rate, date)) * 100) / 100;
}
