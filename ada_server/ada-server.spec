Name: ada-server
Version: 2016.05
Release: 0
Summary: BeeeOn Ada Server
Group: Services
License: BSD
URL: https://github.com/BeeeOn/server/tree/master/ada_server
Source:	server-v%{version}.%{release}.tar.gz
BuildRequires: make

%description
Ada Server provides a remote interface for BeeeOn gateways. The gateways
connects Ada Server to notify changes from sensors and Ada Server controls
actuators connected to the gateways.

%prep
%setup -q -n server-%{version}.%{release}/ada_server

%build
make %{?_smp_mflags}
echo '[Unit]'                         > beeeon-ada-server.service
echo 'Description=BeeeOn Ada Server' >> beeeon-ada-server.service
echo ''                              >> beeeon-ada-server.service
echo '[Service]'                     >> beeeon-ada-server.service
echo 'User=root'                     >> beeeon-ada-server.service
echo 'Type=forking'                  >> beeeon-ada-server.service
echo 'EnvironmentFile=/etc/beeeon/ada-server/environment' >> beeeon-ada-server.service
echo 'WorkingDirectory=/'            >> beeeon-ada-server.service
echo 'ExecStart=/usr/sbin/beeeon-ada-server /etc/beeeon/ada-server/config.xml' >> beeeon-ada-server.service
echo 'PIDFile=/var/run/beeeon-ada-server.pid' >> beeeon-ada-server.service
echo 'ExecReload=pkill -9 --pidfile /var/run/beeeon-ada-server.pid' >> beeeon-ada-server.service
echo 'RestartSec=65'                 >> beeeon-ada-server.service
echo 'Restart=always'                >> beeeon-ada-server.service
echo ''                              >> beeeon-ada-server.service
echo '[Install]'                     >> beeeon-ada-server.service
echo 'WantedBy=multi-user.target'    >> beeeon-ada-server.service

%install
mkdir -p                 $RPM_BUILD_ROOT/usr/sbin
mkdir -p                 $RPM_BUILD_ROOT/etc/beeeon/ada-server
mkdir -p                 $RPM_BUILD_ROOT/usr/lib/systemd/system
cp ada_server            $RPM_BUILD_ROOT/usr/sbin/beeeon-ada-server
cp run_ada_server.sh     $RPM_BUILD_ROOT/usr/sbin/beeeon-ada-server-start.sh
cp ada_server.config.xml $RPM_BUILD_ROOT/etc/beeeon/ada-server/config.xml
cp environment $RPM_BUILD_ROOT/etc/beeeon/ada-server/environment
cp beeeon-ada-server.service $RPM_BUILD_ROOT/usr/lib/systemd/system/beeeon-ada-server.service

%files
/usr/sbin/beeeon-ada-server
/usr/sbin/beeeon-ada-server-start.sh
/etc/beeeon/ada-server/config.xml
/etc/beeeon/ada-server/environment
/usr/lib/systemd/system/beeeon-ada-server.service
