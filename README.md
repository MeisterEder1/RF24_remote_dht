# RF24_remote_dht

Prerequisites / dependencies:
MySQL server with database Monitoring

# SQL-commands to create db and table: #
create database Monitoring;
use Monitoring;
create table TempHumid (ComputerTime INTEGER UNSIGNED,Temperature DECIMAL(5,1), Humidity DECIMAL(5,1), Sensor TINYINT UNSIGNED);

# Install Python packages: #
wget https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py
sudo python ez_setup
sudo easy_install MySQL-python

# Raspbian packages: #
mysql-client
mysql-server
python-mysqldb
libmysqlclient-dev
libblas-dev
liblapack-dev
python-dev
libatlas-base-dev
gfortran
python-setuptools
python-scipy
python-matplotlib

# Libraries: #
wiringPi (git://git.drogon.net/wiringPi)
rf24-bcm (I recommend http://tmrh20.github.io/RF24 as it is mostly up-to-date
