#ifndef BASE_BOND_HPP
#define BASE_BOND_HPP

#include <cstdint>
#include <cassert>
#include <cmath>
#include <vector>
#include <optional>
#include <algorithm>
#include <boost/python.hpp>

#include "cashflow.hpp"
#include "date.hpp"

namespace BondLibrary {
using CashFlows = std::vector<CashFlow>;
using CashFlowsPy = boost::python::list;
using CashFlowOpt = std::optional<const CashFlow>;
class BaseBond {
public:
    BaseBond(
        double face_value,
        double coupon,
        const Date maturity_date,
        const Date issue_date,
        const CashFlowsPy& cashflows,
        const Date settlement_date,
        const DayCountConvention daycount_convention
    );
    virtual ~BaseBond() {}
    double accruedAmount(Date settlement) const;
   // double yieldToMaturity(const double bond_price) const {return yieldToMaturity(bond_price, issue_date_);}
    double yieldToMaturity(const double bond_price, const Date date) const;
    double getCouponRate() const;
    double getCurrentYield(double market_price) const;
    double modifiedDuration(const double rate, const Date date) const;
    bool isExpired() const;
    CashFlowOpt getCashFlow(Date date) const;
    CashFlowOpt getNextCashFlow(const CashFlow& cashflow) const;
    CashFlowOpt getPreviousCashFlow(const CashFlow& cashflow) const;
    virtual double duration(const double rate, const Date date) const = 0;
    double notionalPresentValue(const double rate, Date date) const; 
protected:
    bool outOfRangeOrSlowConvergence(
        double rate_approx,
        double dfroot, 
        double froot, 
        double xh, 
        double xl, 
        double dx_old
    ) const;
    int getCouponFrequency(const Date& date) const;
    double discountFactorYMCount(
        const double year_count, 
        const double day_count, 
        const Date& settlement, 
        const Date& prev_cf_date
    ) const;
    double face_value_;
    double coupon_;
    Date maturity_date_;
    Date issue_date_;
    Date settlement_date_;
    CashFlows cashflows_;
    constexpr static std::vector<double> month_days_ = {
        0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 
    };
    DayCountConvention daycount_convention_;
};
}

#endif
