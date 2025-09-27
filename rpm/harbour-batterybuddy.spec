%define builddir build/%{_target_platform}

Name:       harbour-batterybuddy
Summary:    Battery Buddy
# Keep this in sync with version.h
Version:    4.4.0
Release:    0+beta1
Group:      Qt/Qt
License:    GPLv3
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2

Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   sailfish-version >= 4.3

BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(nemonotifications-qt5)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils
BuildRequires:  libkeepalive-devel

%description
Prolong your battery life.

%prep
%setup -q -n %{name}-%{version}

%build
[ -d %{builddir} ] || mkdir -p %{builddir}
cd %{builddir} || exit 1

%qmake5 ../../
%make_build

%install
cd %{builddir} || exit 1
%qmake5_install

desktop-file-install --delete-original --dir %{buildroot}%{_datadir}/applications %{buildroot}%{_datadir}/applications/*.desktop

install -D -m 644 ${RPM_SOURCE_DIR}/../application/service/%{name}-oneshot.service %{buildroot}%{_unitdir}/%{name}-oneshot.service
install -D -m 644 ${RPM_SOURCE_DIR}/../application/service/%{name}.service %{buildroot}%{_userunitdir}/%{name}.service

%postun
# Run on uninstall, not on upgrade
if [ $1 -eq 0 ]; then
  systemctl --user stop %{name}.service
  systemctl --user disable %{name}.service
  systemctl --user daemon-reload
  systemctl --user reset-failed

  systemctl stop %{name}-oneshot.service
  systemctl disable %{name}-oneshot.service
  systemctl daemon-reload
  systemctl reset-failed
fi

%posttrans
# Remove old service (quietly)
systemctl stop %{name}.service 2>/dev/null ||:
systemctl disable %{name}.service 2>/dev/null ||:
rm -f %{_unitdir}/%{name}.service

systemctl daemon-reload
systemctl --user daemon-reload

# Install/update permission daemon (root)
systemctl enable %{name}-oneshot.service
systemctl stop %{name}-oneshot.service
systemctl start %{name}-oneshot.service

# Install/update background daemon (user)
systemctl --user enable %{name}.service
systemctl --user stop %{name}.service
systemctl --user start %{name}.service

%files
%{_bindir}/%{name}
%{_bindir}/%{name}-daemon
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
%{_userunitdir}/%{name}.service
%{_unitdir}/%{name}-oneshot.service
