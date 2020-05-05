#include "udf.h"

DEFINE_DELTAT(autosave,d)
{
   real dt_next = 0.;
   real dt = CURRENT_TIMESTEP;
   real dt_autosave = RP_Get_Real("dt_autosave");
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
            phi = fabs(C_T(c,t) - C_T_M1(c,t))/(1+MIN(fabs(C_T(c,t)),fabs(C_T_M1(c,t))));
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

   /* Adjust time step for autosaving */
   if (dt_autosave > 0)
   {
      t_to_save = dt_autosave - (t_curr - dt_autosave*floor(t_curr/dt_autosave));
      if (dt_next >= t_to_save)
      {
         dt_next = t_to_save;
         RP_Set_Integer("write_now",1);
      }
   }

   return dt_next;
}
