#ifndef UTILITY_H
#define UTILITY_H

#define READ_BIT(REG, BIT)  ((REG) & (BIT))
#define WRITE_REG(REG, VAL) ((REG) = (VAL))
#define SET_BIT(REG, BIT)   ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))

#endif // UTILITY_H