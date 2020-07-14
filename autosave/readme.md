The autosave utility is based on the DEFINE_DELTAT macro in ANSYS FLUENT.  

The time step is adjusted based on the change of field variables of interest (temperature in this case).

The utility checks if the next time step is greater than the time until the next autosave event and adjusts the time step, if needed, in order to capture the autosave.

The solution will be saved at exactly the specified time intervals.
