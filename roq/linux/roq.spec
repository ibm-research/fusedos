Summary: RoQ Driver and init-script
Name: bgas-roq-module
Version: 1
Release: 1
License: GPL
#Group: 
#URL: 

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
Packaging the RoQ driver.

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/lib/modules/%{ROQ_KERNEL}/extra
cp %{CURR_DIR}/roq.ko $RPM_BUILD_ROOT/lib/modules/%{ROQ_KERNEL}/extra

mkdir -p $RPM_BUILD_ROOT/etc/rc.d/init.d
cp %{CURR_DIR}/bgroq $RPM_BUILD_ROOT/etc/rc.d/init.d
cp %{CURR_DIR}/bgbgashosts $RPM_BUILD_ROOT/etc/rc.d/init.d

%clean
rm -rf $RPM_BUILD_ROOT
echo Clean

%post
depmod -a %{ROQ_KERNEL}

ln -s ../init.d/bgroq /etc/rc.d/rc3.d/K98bgroq
ln -s ../init.d/bgroq /etc/rc.d/rc3.d/S01bgroq
ln -s ../init.d/bgbgashosts /etc/rc.d/rc3.d/S11bgbgashosts


%files
%defattr(-,root,root,-)
/lib/modules/%{ROQ_KERNEL}/extra/roq.ko
/etc/rc.d/init.d/bgroq
/etc/rc.d/init.d/bgbgashosts
%doc


%changelog
* Thu Oct  4 2012 IBM
- Initial build.

