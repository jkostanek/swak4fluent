#include "udf.h" 
#include "unsteady.h" 

DEFINE_UDS_UNSTEADY(uds_unsteady_first_order,c,t,i,apu,su)
{
	/* This function alters the time-derivative term in FLUENT UDS equation */
	real dt, vol, phi_M1;
	dt = CURRENT_TIMESTEP;
	vol = C_VOLUME(c,t);
  	
	*apu = -vol/dt;/*implicit part*/
	phi_M1 = C_UDSI_M1(c,t,i);
	*su  = vol*phi_M1/dt;/*explicit part*/

}

DEFINE_UDS_UNSTEADY(uds_unsteady_second_order,c,t,i,apu,su)
{
	/* This function alters the time-derivative term in FLUENT UDS equation */
	real dt, dt_M1, R, vol, phi_M1, phi_M2;
	dt = CURRENT_TIMESTEP;
	vol = C_VOLUME(c,t);
  	
	if (N_TIME < 2)
	{
		*apu = -vol/dt;/*implicit part*/
		phi_M1 = C_UDSI_M1(c,t,i);
		*su  = vol*phi_M1/dt;/*explicit part*/
	}
	else
	{
		dt_M1 = PREVIOUS_TIMESTEP;
		R = pow((dt + dt_M1),2)/pow(dt,2);
		*apu = -vol*(R-1)/(R*dt-(dt+dt_M1));
		phi_M1 = C_UDSI_M1(c,t,i);
		phi_M2 = C_UDSI_M2(c,t,i);
		*su  = vol*phi_M1*R/(R*dt-(dt+dt_M1)) - vol*phi_M2/(R*dt-(dt+dt_M1));
	}
}

DEFINE_DELTAT(adaptive_time_step,d)
{
   real dt_next = 0.;
   real dt = CURRENT_TIMESTEP;
   real dt_min = RP_Get_Real("dt_min");
   real dt_max = RP_Get_Real("dt_max");
   real t_curr = CURRENT_TIME;
   real t_to_save = 0.;
   real f_min = RP_Get_Real("f_min");
   real f_max = RP_Get_Real("f_max");
   real tol = RP_Get_Real("tol");
   real phi = 0.;
   real max_phi = 0.;
   
   #if !RP_HOST
      Thread *t;
      cell_t c;

      thread_loop_c(t,d)
      {
         begin_c_loop_int(c,t)
         {
            phi = fabs(C_UDSI(c,t,0) - C_UDSI_M1(c,t,0))/(1+MIN(fabs(C_UDSI(c,t,0)),fabs(C_UDSI_M1(c,t,0))));
            if (phi > max_phi)
               max_phi = phi;
         }
         end_c_loop_int(c,t)
      }
   
      #if RP_NODE
         max_phi = PRF_GRHIGH1(max_phi);
      #endif
   #endif

   node_to_host_real_1(max_phi);
   
   /* Adjust time step to meet tolerance requirements and min/max time step sizes */
   dt_next = dt*MIN(MAX(tol/max_phi,f_min),f_max);
   dt_next = MAX(MIN(dt_next,dt_max),dt_min);

   return dt_next;
}
