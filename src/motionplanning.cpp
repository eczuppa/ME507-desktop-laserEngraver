// create the ramp inputs from the kinematics module to send to the contorller
// needs to caluclate the time it takes go to the posiiton and will use the current task timing to
// divide the total move into smaller linear movements and speeds so deviations from set point will be
// small which is better suited for the classical controls style controller we are using.