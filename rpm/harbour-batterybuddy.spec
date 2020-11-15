Name:       harbour-batterybuddy

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

# Change the version and release strings in .pro file only!

Summary:    Battery Buddy
Version:    3.6
Release:    1
Group:      Qt/Qt
License:    GPLv3
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   lipstick-qt5-tools
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils

%description
Prolong your battery life.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
# >> files
# << files

%posttrans
# Remove all service (new and old)
systemctl stop %{name}-oneshot.service || true
systemctl stop %{name}.service || true
su nemo -c "systemctl --user stop %{name}.service" || true

systemctl disable %{name}-oneshot.service || true
systemctl disable %{name}.service || true
su nemo -c "systemctl --user disable %{name}.service" || true

rm %{_unitdir}/%{name}-oneshot.service || true
rm %{_unitdir}/%{name}.service || true
rm %{_userunitdir}/%{name}.service || true
rm /etc/systemd/system/%{name}.service || true
rm /etc/systemd/system/%{name}-oneshot.service || true

# Install/update permission daemon (root)
cp %{_datadir}/%{name}/service/%{name}-oneshot.service %{_unitdir}/%{name}-oneshot.service
systemctl start %{name}-oneshot.service
systemctl enable %{name}-oneshot.service

# Install/update background daemon (nemo)
cp %{_datadir}/%{name}/service/%{name}.service %{_userunitdir}/%{name}.service
su nemo -c "systemctl --user start %{name}.service"
su nemo -c "systemctl --user enable %{name}.service"

# Cleanup
systemctl daemon-reload
systemctl reset-failed


%postun
su nemo -c "systemctl --user disable --now %{name}.service" || true
systemctl disable --now %{name}-oneshot.service || true
rm %{_unitdir}/%{name}-oneshot.service
rm %{_userunitdir}/%{name}.service
