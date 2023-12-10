#ifndef CASHFLOW_HPP
#define CASHFLOW_HPP

#include <cstdint>

#include "date.hpp"

namespace BondLibrary {
struct CashFlow {
    CashFlow(double cashflow, Date due_date)
        : cashflow(cashflow), due_date(due_date)
    {}
    double cashflow = 0; // amount to be received
    Date due_date; // date on which the cashflow is due to be paid out
    bool operator<(CashFlow rhs) {
        return rhs.due_date < due_date;
    }
};   
}

#endif
