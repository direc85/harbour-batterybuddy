# harbour-batterybuddy
Sailfish OS application that alerts the user when the battery charge level gets too high or too low. This helps in increasing the battery lifespan. There are a *lot* of research material on this, the numbers I pulled into the application as a reference were pulled from BatteryUniverse article BU-808: https://batteryuniversity.com/learn/article/how_to_prolong_lithium_based_batteries

There is no service/daemon support at the moment, because that is not allowed in Jolla Store. You have to open the app and keep it open in the background in order for it to work. I can create another version that runs also as a service, if there is demand for it. The upside of this approach is that whenever the alerts start to annoy, simply closing the application silences the sounds and disables notifications.

Another known "issue" at the moment is that the alert sound played uses the media volume setting instead of ringtone volume setting. Any tips on this are appreciated.

Keep your battery healthy and help our dying planet, even if it is just for a little tiny bit!
