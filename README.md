# harbour-batterybuddy
Battery Buddy is a Sailfish OS application that pauses and resumes charging at set percentage values, or alerts the user when the battery charge level gets too high or too low. This helps in increasing the battery lifespan. There are a *lot* of research material on this, the numbers I pulled into the application as a reference were pulled from [Battery Universe online article BU-808](https://batteryuniversity.com/learn/article/how_to_prolong_lithium_based_batteries).

The application logo is a 5 minute craft at the moment, and can be considered a permanent temporary solution. It doesn't look nice in the notification view. If you have the skills to make a better logo using the same idea (a heart inside the battery) and are willing to spend a little time for this project, please feel free to send a new logo to me.

The application consist of three parts: the GUI, the monitoring daemon and the filesystem permission daemon. The filesystem daemon, `harbour-batterybuddy-oneshot.service`, always starts when the device boots. Its sole purpose is to set correct permissions for the charger control file of the device, so that everything else can be started as user `nemo`, minimizing security impact. The monitoring daemon, `harbour-batterybuddy.service`, actually sends the notifications and pauses/resumes charging.

Another known "issue" at the moment is that the alert sound played uses the media volume setting instead of ringtone volume setting. Any tips on this are appreciated.

Keep your battery healthy and help our dying planet, even if it is just for a tiny little bit!
