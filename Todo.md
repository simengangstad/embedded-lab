+ Free memory in gui
+ Specify macros better
+ Document and structurize 
+ Interrupt for joystick button
+ put void in emty parameter lists
+ ADC timing (joystick, delay)
+ Improve CAN message
+ Use const more
+ Macro in can controller init for MCP values
+ Menu goes above top item
+ Left shift macros instead of hex
+ Add test for each day (for evaluation)
+ Make a joystick type with direction and position(?)
+ Add joystick button to Joystick struct, so we can send everything at once. Make function for atmega to send Joystick struct and everything lse from USB-card over can (eg. sliders). Make explicit cast from int8_t to uint8_t
+ Skal sliderene styre servo, og så x-aksen på joystick styre posisjon?
+ Joystick is too unstable and flickering. 


Fra forrige lab: Testing Open loop: 
+ Veit at DIR ikke endres ved endring i x-position. 
+ Når x er negativ (til venstre) så holder ikke signalet konstant verdi og er lav. Høyre gir nesten 5V (AOUT). 