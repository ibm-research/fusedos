# on interactive shells /etc/profile is already sourced
if [[ $- != *i* ]]; then
    test -r /etc/profile && . /etc/profile
fi

# the following is required for mpirun_rsh to work
if [ -r /opt/mvapich2/etc/env.roq ]; then
    . /opt/mvapich2/etc/env.roq
fi

alias ll='ls -la'
