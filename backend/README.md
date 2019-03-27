# MiFLo backend

These two tools send commands to the MiFlo. One is a Google Calendar script that sends the 5 upcoming events on a calendar to the MiFlo, to be cached and executed when planned. The second is a Telegram bot that listens to commands over Telegram and forwards them to the MiFLo.

## Calendar

Follow ruby calendar quickstart at [https://developers.google.com/calendar/quickstart/ruby] (https://developers.google.com/calendar/quickstart/ruby) to obtain credentials in the form of a `client_secret.json` file so you can run the example:

`ruby calendar.rb -c CALENDAR_ID -p PERSON_NAME -h MQTT_BROKER`

## Telegram bot

Get a Telegram bot up and running, acquire the token and run the MiFlo bot:

`ruby telegram_bot.rb -h MQTT_BROKER -t TELEGRAM_BOT_TOKEN -p PERSON_NAME`


## Example installation rapsberry pi

The backend can run on any server, but here is a small example on how to do it on a raspberry pi. See below for an example on a virtual appliance:

* Install raspbian [https://www.raspberrypi.org/downloads/raspbian/](https://www.raspberrypi.org/downloads/raspbian/)
* Using `sudo raspi-config` you can adapt some basic settings (e.g. enable ssh, change hostname, update, ...), do not forget to correct the timezone
* `sudo apt-get update`
* install git: `sudo apt-get install git`
* clone the repo `git clone https://github.com/TeamScheire/MiFlo.git`
* `cd MiFlo/backend`
* install ruby: 
 ```
 sudo apt-get install ruby
 sudo gem install bundler
 bundle install
 ```
* install mosquitto broker:
 ```
 sudo wget https://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
 sudo apt-key add mosquitto-repo.gpg.key
 cd /etc/apt/sources.list.d/
 sudo wget http://repo.mosquitto.org/debian/mosquitto-stretch.list
 sudo apt-get update
 sudo apt-get install mosquitto mosquitto-clients
 ```

* get the calender up and running
 * Follow ruby calendar quickstart at [https://developers.google.com/calendar/quickstart/ruby] (https://developers.google.com/calendar/quickstart/ruby) to obtain credentials in the form of a `client_secret.json`
 * copy/rename the `client_secret.json` to the backend folder, e.g. using `scp`
 * test the calender script `ruby calendar.rb -c CALENDAR_ID -p PERSON_NAME -h localhost` 
  where CALENDER_ID can be found in the settings of the google calender
 * add it to crontab `crontab -e`
 * e.g. every 5 minutes: `*/5 * * * * script -c "cd /home/pi/MiFlo/backend/; ruby calendar.rb -c CALENDAR_ID -p PERSON_NAME -h localhost" /home/pi/MiFlo/backend/calendar.log`
* get the telegram bot up and running
 * create a new bot: `https://core.telegram.org/bots#6-botfather` 
 * test the script `ruby telegram_bot.rb -h localhost -t TELEGRAM_BOT_TOKEN -p PERSON_NAME
 * start on startup: `sudo nano /etc/rc.local`
 * add line `ruby /home/pi/MiFlo/backend/telegram_bot.rb -h localhost -t TELEGRAM_BOT_TOKEN -p PERSON_NAME & > /home/pi/MiFlo/backend/telegram_bot.log  2>&1`

## Example installation on a openSUSE Linux server / virtual appliance

If you already have a home server running you can also add the MiFlo backend onto that server easily. If it is a Linux server it could be installed directly on it or, and in any other case, you can easily create a virtual appliance to run in KVM, Xen, MS HyperV, VMware, VirtualBox, ...

* Download an openSUSE JeOS image (Just enough OS) [here](https://software.opensuse.org/distributions/leap#JeOS-ports)
* Create a new VM (1vCPU, 512M Ram should do) in your favourite virtualization platform and assign the downloaded JeOS image as primary disk
* Start the VM
* Configure your network:
  * set hostname:
  ```
  hostnamectl set-hostname <hostname>
  ```
  where `<hostname>` is your desired hostname
  * set IP address:
  ```
  vi /etc/sysconfig/network/ifcfg-eth0
  ```
   * For DHCP, everything is probably already set.. but check if `BOOTPROTO=auto` is set
   * For a static IP config:
    * Set the IP:
    ```
    STARTMODE=auto
    BOOTPROTO=static
    IPADDR=<x.x.x.x>/<XX>
    ```
    where `<x.x.x.x>` is the IP address and `<XX>` the subnet mask in CIDR notation (ex. `24` for `255.255.255.0`)
    * Set the default route, so the machine can access the internet:
    ```
    vi /etc/sysconfig/network/routes
    ```
    ```
    default <x.x.x.x> - -
    ```
    where `<x.x.x.x>` is your default gateway
    * Set DNS servers
    ```
    vi /etc/sysconfig/network/config
    ```
    ```
    ...
    NETCONFIG_DNS_STATIC_SEARCHLIST=<domain name>
    ...
    NETCONFIG_DNS_STATIC_SERVERS=<dns servers>
    ...
    ```
    where `<domain name>` is your local search domain, if any, and `<dns servers>` a space delimited list of DNS servers.
   * Restart the network
     ```
     systemctl restart network
     ```
* Install an NTP client to make sure the clock is set correct:
  ```
  zypper install chrony
  ```
  * and add your favourite NTP servers to `/etc/chrony.conf`:
    ```
    server 0.be.pool.ntp.org
    server 1.be.pool.ntp.org
    server 2.be.pool.ntp.org
    server 3.be.pool.ntp.org
    ```
  * Enable and start the chrony service:
    ```
    systemctl enable chronyd
    ```
* Install required dependencies for the MiFlo backend:
  ```
  zypper install sudo git ruby ruby2.5-rubygem-bundler mosquitto mosquitto-clients
  ```
* Enable and start the mosquitto MQTT broker:
  ```
  systemctl enable mosquitto
  systemctl start mosquitto
  ```
* Open firewall port `1883/tcp`:
  ```
  firewall-cmd --permanent --zone=public --add-port=1883/tcp
  ```
* Create a `miflo` user which will run the backend scripts:
  ```
  useradd -m miflo
  ```
* Switch to the `miflo` user
  ```
  sudo -i -u miflo
  ```
  * Clone the miflo git repository
    ```
    git clone https://github.com/TeamScheire/MiFlo.git
    cd MiFlo/backend
    ```
  * Install required ruby dependencies:
    ```
    bundle install
    ```
  * Configure the calendar script:
   * Follow ruby calendar quickstart at https://developers.google.com/calendar/quickstart/ruby to obtain credentials in the form of a `client_secret.json`-file.
   * copy/rename the `client_secret.json` to the backend folder, e.g. using `scp`
   * test the calender script `ruby calendar.rb -c CALENDAR_ID -p PERSON_NAME -h localhost` where CALENDER_ID can be found in the settings of the google calender
  * Configure the Telegram script:
   * create a new bot: https://core.telegram.org/bots#6-botfather
   * test the script `ruby telegram_bot.rb -h localhost -t TELEGRAM_BOT_TOKEN -p PERSON_NAME
  * Now switch back to the `root`-user:
    ```
    exit
    ```
* Create a service for the calendar script:
  * Create a systemd service unit for the calendar script:
    ```
    vi /etc/systemd/system/miflo_calendar\@<PERSON_NAME>.service
    ```
    where `<PERSON_NAME>` is the name of the miflo end-user (create such a file for each miflo device this backend will service)
    ```
    [Unit]
    Description=Retrieve MiFlo Calendar events for Bernd
      
    [Service]
    WorkingDirectory=/home/miflo/MiFlo/backend
    User=miflo
    Type=oneshot
    ExecStart=/usr/bin/ruby calendar.rb -c <CALENDAR_ID> -p <PERSON_NAME> -h localhost
    ```
    where `<PERSON_NAME>` is the name of the miflo end-user and `<CALENDAR_ID>` the ID of the Google Calendar for this end-user.
  * Create a systemd timer unit for the calendar script as this script should run every 5 minutes:
    ```
    vi /etc/systemd/system/miflo_calendar\@.timer
    ```
    ```
    [Unit]
    Description=Retrieve MiFlo Calendar events for %i every 5 minutes
    
    [Timer]
    OnCalendar=*:0/5
    
    [Install]
    WantedBy=multi-user.target
    ```
   * Enable and start the timer:
     ```
     systemctl enable miflo_calendar\@<PERSON_NAME>.timer
     systemctl start miflo_calendar\@<PERSON_NAME>.timer
     ```
     where `<PERSON_NAME>` is the name of the miflo end-user. This is a generic timer unit and it will call the `miflo_calendar@<PERSON_NAME>.service` unit every 5 minutes. So you can repeat these commands for every end-user you provided a .service unit for.
   * You can now check the status of the timer using
     ```
     systemctl list-timers
     ```
     where the `miflo_calendar@PERSON_NAME.timer` should be listed. Here you can see when it was last runned, and when the next run will take place.
   * You can check the status of the last run of the script using
     ```
     systemctl status miflo_calendar\@<PERSON_NAME>.service
     ```
     and check the logging with
     ```
     journalctl -u miflo_calendar\@<PERSON_NAME>.service
     ```
* Create a service for the telegram bot-script
  * Create a systemd service unit:
    ```
    vi /etc/systemd/system/miflo_telegrambot\@.service
    ```
    ```
    [Unit]
    Description=MiFlo Telegram Bot for %i
    
    [Service]
    WorkingDirectory=/home/miflo/MiFlo/backend
    User=miflo
    #Group=miflo
    Restart=on-failure
    RestartSec=10
    ExecStart=/usr/bin/ruby telegram_bot.rb -h localhost -t <TELEGRAM_BOT_TOKEN> -p %i
    
    [Install]
    WantedBy=multi-user.target
    ```
    where `<TELEGRAM_BOT_TOKEN>` is your Telegram bot token.
  * Enable and start the service:
    ```
    systemctl enable miflo_telegrambot\@<PERSON_NAME>.service
    systemctl start miflo_telegrambot\@<PERSON_NAME>.service
    ```
    where `<PERSON_NAME>` is the name of the miflo end-user
  * You can now check the status of the service
     ```
     systemctl status miflo_telegrambot\@<PERSON_NAME>.service
     ```
     and check the logging with
     ```
     journalctl -u miflo_telegrambot\@<PERSON_NAME>.service
     ```
And that's it. The virtual appliance should now process any messages sent to the Telegram bot and events on Google calendar and forward to the MiFlo device if it is connected to this backend.
 
 Also make sure the appliance is set to autostart on your server.
