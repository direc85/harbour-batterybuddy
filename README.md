# harbour-batterybuddy

<img src="https://github.com/direc85/harbour-batterybuddy/raw/master/screenshot1.jpg?raw=true" alt="Screenshot 1" style="width: 270px; height: 480px;"> <img src="https://github.com/direc85/harbour-batterybuddy/raw/master/screenshot4.jpg?raw=true" alt="Screenshot 4" style="width: 270px; height: 480px;">

Battery Buddy is a Sailfish OS application that pauses and resumes charging at set percentage values, or alerts the user when the battery charge level gets too high or too low. This helps in increasing the battery lifespan. There are a *lot* of research material on this, the numbers I pulled into the application as a reference were pulled from [Battery Universe online article BU-808](https://batteryuniversity.com/learn/article/how_to_prolong_lithium_based_batteries).

The application consist of three parts: the GUI, the monitoring daemon and the filesystem permission daemon. The filesystem daemon, `harbour-batterybuddy-oneshot.service`, always starts when the device boots. Its sole purpose is to set correct permissions for the charger control file of the device, so that everything else can be started as user `defaultuser` (or `nemo`), minimizing security impact. The monitoring daemon, `harbour-batterybuddy.service`, actually sends the notifications and pauses/resumes charging.

There's a logging feature, too, which writes to a simple text log. Depending on the log level it can log percentage changes, temperature changes and momentary device current draw. Note that the measurement can only be done when the device is not in deep sleep mode, so the power draw reading can never reflect the true lowest possible values.

You can download Battery Buddy using [Storeman](https://openrepos.net/content/olf/storeman-installer), or directly from  [Openrepos](https://openrepos.net/content/direc85/battery-buddy). For support, open a ticket or check out the [Battery Buddy support thread](https://forum.sailfishos.org/t/battery-buddy-support-thread/12580/47) over at Sailfish OS forum!

Keep your battery healthy and help our dying planet, even if it is just for a tiny little bit!
