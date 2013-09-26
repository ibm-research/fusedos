Summary: RoQ Library
Name: bgas-roq-library
Version: 1
Release: 1
License: GPL
#Group: 
#URL: 

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
Packaging the RoQ user-space libraries.

%install
install -D -m 0644 %{ROQLIB_DIR}/roq.driver $RPM_BUILD_ROOT/etc/libibverbs.d/roq.driver
install -D -m 0755 %{ROQLIB_DIR}/src/.libs/libroq.so $RPM_BUILD_ROOT/opt/roq/lib64/libroq-rdmav2.so

mkdir -p $RPM_BUILD_ROOT/etc/ld.so.conf.d
echo "/opt/roq/lib64" > $RPM_BUILD_ROOT/etc/ld.so.conf.d/roq.conf


%clean
rm -rf $RPM_BUILD_ROOT
echo Clean

%post
ldconfig

%files
%defattr(-,root,root,-)
%dir /opt/roq
%dir /opt/roq/lib64
/etc/libibverbs.d/roq.driver
/etc/ld.so.conf.d/roq.conf
/opt/roq/lib64/libroq-rdmav2.so
%doc

%changelog
* Thu Oct  4 2012 IBM
- Initial build.

