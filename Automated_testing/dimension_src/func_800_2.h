#ifdef F800_2
//2 800
float r10 = 1.0F/(vpL0(x + 2, y + 2, z + 2)*vpL0(x + 2, y + 2, z + 2));
uL0(t2, x + 2, y + 2, z + 2) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 2, y + 2, z + 2) + r10*(-r8*(-2.0F*uL0(t0, x + 2, y + 2, z + 2)) - r8*uL0(t1, x + 2, y + 2, z + 2)) + 4.44444e-3F*(uL0(t0, x + 1, y + 2, z + 2) + uL0(t0, x + 2, y + 1, z + 2) + uL0(t0, x + 2, y + 2, z + 1) + uL0(t0, x + 2, y + 2, z + 3) + uL0(t0, x + 2, y + 3, z + 2) + uL0(t0, x + 3, y + 2, z + 2)) - 2.66666673e-2F*uL0(t0, x + 2, y + 2, z + 2))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
#endif