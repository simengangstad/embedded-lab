Genrelt:
+ Fiks Joystick
+ Gjør CAN-grensesnittene for Atmega og Atsam like
+ Free memory in gui
+ ADC timing (joystick, delay)
+ Left shift macros instead of hex
+ Add test for each day (for evaluation)

Regulator:
+ ISR må være kort, så vi bør oppdatere pådraget utenfor, altså i main-loopen
+ Finne et fornuftig timestep
+ Joystick is too unstable and flickering

Menu system:
+ Use buttons for quit and reset when playing
+ Make profiles and scoreboard, store highscore and users in program memory
+ Menu goes above top item
+ Play-funksjonen sender CAN-message for å aktivere Atsam, går deretter i en while loop som kjører helt til quit-knappen trykkes, og hvor vi kontinuerlig sender Joystick og sliders til Atsam.