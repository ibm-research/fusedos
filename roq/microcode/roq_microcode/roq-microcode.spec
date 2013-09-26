Summary: RoQ Microcode
Name: bgas-roq-microcode
Version: 1
Release: 1
License: GPL
#Group: 
#URL: 

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
Packaging the RoQ microcode.

%install
mkdir -p $RPM_BUILD_ROOT/boot
cp %{ROQMICRO_DIR}/roq_microcode $RPM_BUILD_ROOT/boot
chmod 0755 $RPM_BUILD_ROOT/boot/roq_microcode

%clean
rm -rf $RPM_BUILD_ROOT
echo Clean

%post


%files
%defattr(-,root,root,-)
/boot/roq_microcode
%doc


%changelog
* Thu Oct  4 2012 IBM
- Initial build.

