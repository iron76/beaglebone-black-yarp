
if [ "$1" != "" ]; then
    CMD_PATH=`which $1`
    echo "Setting privilege for" $CMD_PATH 
    sudo chown root $CMD_PATH
    sudo chmod 4755 $CMD_PATH
    echo "Privileges should be set"
else
    echo ""
    echo "Use of this script is: ./gpio_sudo_privilege.sh command_which_should_have_sudo_privilege"
    echo "E.g. ./gpio_sudo_privilege.sh /home/iron/Code/bin/bin/run_athletebot"
    echo "Contact: iron@liralab.it (Francesco Nori)"
fi

