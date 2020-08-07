  
The uds_unsteady_second_order utility is based on the user-defined, DEFINE_UDS_UNSTEADY macro in ANSYS FLUENT.  

The utility can be used for the unsteady term of the scalar transport equation used with user-defined-scalars (UDS).

Note that the unsteady term in this utility is used to solve the diffusion equation d(phi)/dt = D*d(phi)/dx^2.  Instead of entering the diffusion coefficient (kg/m-s), the scalar diffusivity "D" should be entered with units (m^2/s).  

If the problem involves advection, then this utility should be modified so that the transient term is d(rho*phi)/dt, in which case the diffusion coefficient would be entered as usual (kg/m-s).
