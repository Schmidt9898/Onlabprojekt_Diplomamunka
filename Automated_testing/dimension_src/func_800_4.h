#ifdef F800_4
//4 800
float r10 = 1.0F/(vpL0(x + 4, y + 4, z + 4)*vpL0(x + 4, y + 4, z + 4));
uL0(t2, x + 4, y + 4, z + 4) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 4, y + 4, z + 4) + r10*(-r8*(-2.0F*uL0(t0, x + 4, y + 4, z + 4)) - r8*uL0(t1, x + 4, y + 4, z + 4)) + 3.70370379e-4F*(-uL0(t0, x + 2, y + 4, z + 4) - uL0(t0, x + 4, y + 2, z + 4) - uL0(t0, x + 4, y + 4, z + 2) - uL0(t0, x + 4, y + 4, z + 6) - uL0(t0, x + 4, y + 6, z + 4) - uL0(t0, x + 6, y + 4, z + 4)) + 5.92592607e-3F*(uL0(t0, x + 3, y + 4, z + 4) + uL0(t0, x + 4, y + 3, z + 4) + uL0(t0, x + 4, y + 4, z + 3) + uL0(t0, x + 4, y + 4, z + 5) + uL0(t0, x + 4, y + 5, z + 4) + uL0(t0, x + 5, y + 4, z + 4)) - 3.33333341e-2F*uL0(t0, x + 4, y + 4, z + 4))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
#endif