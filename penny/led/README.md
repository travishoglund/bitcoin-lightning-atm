# Bitcoin Penny ATM (LED)

#### This ATM can be built for about `$150 USD` in materials

[![Penny LED ATM](../../images/led-atm.jpg)](../../images/led-atm-large.jpg)

`Pros`: Professional/Modern looking, save more of your SATS from sale

`Cons`: Only accepts a single coin as input (penny)


## Video Walkthrough
[https://www.youtube.com/watch?v=A0-by4YgqxY](https://www.youtube.com/watch?v=A0-by4YgqxY)

## Required Parts:

![Parts](../../images/led-parts.jpg)

- [Coin Acceptor](https://amzn.to/421g9J6)
- [ESP32 Micro Controller](https://amzn.to/40ixlse)
- [12V to 5V USB Adaptor](https://amzn.to/3L7NEUk)
- [12V Toggle Switch](https://amzn.to/3UNaFzg)
- [12V Battery Pack](https://amzn.to/3H1W39t)
- [Breadboard](https://amzn.to/40iYhb4)
- [10K Resister](https://amzn.to/40sGIFH)
- [Shrink Tubing](https://amzn.to/3ojl0H0)
- [Breadboard Jumper Wires](https://amzn.to/3GPbUIl)
- [AA Batteries x 8](https://amzn.to/41vrUqK)

## Recommended Tools

![Tools](../../images/tools.jpg)

## Assembly Instructions

### Print the 3D parts
1. Print the [3D Box](../../3D/body.stl), then print the [Slotted Side Panel](../../3D/lid.stl).
   ####
   ![Printed Box](../../images/box.jpg)

### LNBits Wallet Setup
- Go to [LNBits](https://legend.lnbits.com/) and create a wallet (give it whatever name you wish). Once created, it will show a warning that the wallet is only secured by this URL.  Copy the URL.
- In the root of this project, go into the qrcodejs folder and open index.html.  Paste your wallet URL and it will generate a QR code for you to print.  I suggest taping this to the back of a post it note and putting it inside of your ATM for safekeeping. Deposit whatever you are comfortable with (I recommend 5,000 sats).
- `NOTE: This method of storing BTC is not secure and I only recommend putting 10,000 sats or less on this wallet just for demonstrating the functionality of the ATM to your friends/family on the go.  Keep your SATS safe at all times :)`
- Go to the extensions tab and add/enable "LNURLDevice", then click on it in the sidebar menu.
- Enter in the following information.  ![LNBits Configuration](../../images/lnbits-configuration.png)
- On this screen, make sure to copy the id and key - we will use these in our code in the next section. ![LNBits Configuration](../../images/lnbits-device-info.png)

### Micro Controller Configuration
While that is printing, lets prepare the micro controller.
- Download the [Arduino IDE](https://www.arduino.cc/en/software)
- Once installed, connect the micro controller to your computer via USBC, and from the dropdown select "Select other board and port...", and select the "ESP32 Dev Module" and on the right select your USB port that it is connected to.
- Copy and paste the code from the arduino-code folder into the editor. Replace the "DEVICE_ID" with our id from the previous section, and "SECRET_CODE" with our key from the previous section.
- Click the upload button (the second button on the top left).  Once it uploads to the micro controller you should see the Bitcoin ATM screen.  It can safely be removed from the computer and is ready for the assembly into our box.

### Coin Acceptor Configuration
On the side of the coin acceptor there is a slot for a coin to be added.  Slide the sample coin out and insert a penny into the slot - make sure it is seated securely.

![Coin Configuration](../../images/penny-configuration.jpg)

SW1 - N.O

SW2 - 50ms

SW3 - NOM

### Assembling the ATM
Now we get to put everything together and make this functional!


#### Assemble the LCD Screen
- Start by taking apart the LCD panel. There are tiny little tabs that lock into the metal plate so gently pry them until you can get the top and bottom separated.<br /> ![LCD Step 1](../../images/lcd-step-1.jpg)
- Before assembling it into the ATM, ensure your printer made the LCD hole perfectly straight.  If necessary, shave the edges to be straight before inserting. Insert it through the top of the ATM at an angle.  Get the steel plate situated first, then place the top down on top of it.<br /> ![LCD Step 2](../../images/lcd-step-2.jpg)
- Flip the ATM upside down and press so it makes a tight fit.<br /> ![LCD Step 3](../../images/lcd-step-3.jpg)
- Plug the USBC cable into the micro controller, then gently lift the cable and press it down with some hot glue (do not get it on the board).  This will hold the LCD firmly in place so it never wants to come apart.<br /> ![LCD Step 4](../../images/lcd-step-4.jpg)

#### Assemble the Toggle Switch
- On to the toggle switch! There are 3 pins on the toggle switch, the silver is the ground (black), and the two golds are power (red).<br /> ![Toggle Step 1](../../images/toggle-step-1.jpg)
- Solder a black wire onto the silver pin, and two red wires on the gold pins.<br /> ![Toggle Step 2](../../images/toggle-step-2.jpg)
- Slide shrink tubing onto the wires and shrink them with a lighter.<br /> ![Toggle Step 3](../../images/toggle-step-3.jpg)
- Smooth out the 3D hold with a utility knife and then slide the toggle switch into the device.<br /> ![Toggle Step 4](../../images/toggle-step-4.jpg)
- Place the nut through the wires and spin it onto the toggle switch.  I prefer the LED of the toggle switch to be facing downward, but it doesn't really matter for functionality.<br /> ![Toggle Step 5](../../images/toggle-step-5.jpg)

#### Assemble the Coin Acceptor
- Plug the coin acceptor wires into the back of the device (snip the gray wires), and place it through the front of the box.  Secure it loosely with the top two bolts.<br /> ![Coin Acceptor Step 1](../../images/acceptor-step-1.jpg)
- Secure the device with the bottom two bolts and tighten when everything aligns squarely on the front of the device.<br /> ![Coin Acceptor Step 2](../../images/acceptor-step-2.jpg)

#### Assembling the Breadboard
- Prepare the breadboard by peeling off the sticky backing. <br /> ![Breadboard Step 1](../../images/breadboard-step-1.jpg)
- Place the breadboard in the upper right of the box.  You will want to place it so that all of the holes are accessible <br /> ![Breadboard Step 2](../../images/breadboard-step-2.jpg)

#### Connecting the 12V to 5V Power Adapter
- Prepare the 12V step down adapter, and grab some super glue or hot glue. <br /> ![12V Step 1](../../images/12v-step-1.jpg)
- Apply some glue to the back of the adapter. <br /> ![12V Step 2](../../images/12v-step-2.jpg)
- Orientation is important!  Place the adapter just below the toggle switch nut with the red and black wires facing the outside opening of the box.  The USB wire should naturally want to wrap around the toggle wires.  Make sure not to block the panel slot when fixing this to the box. <br /> ![12V Step 3](../../images/12v-step-3.jpg)
- Wrap the excess USB wire around the toggle nut wires (2x) and then glue it to the top of the adapter box.  This holds everything nice and secure for us and still makes it accessible. <br /> ![12V Step 4](../../images/12v-step-4.jpg)
- You can now plug in the white USB cable to the 12V adapter.

#### Wiring the device
- The wiring it fairly straightforward, just make sure to save the red wire coming from the center pin of the toggle switch, as that will go directly to our battery bank.  Here is a couple images showcasing the wiring diagram. <br /> ![Wiring Configuration 1](../../images/wiring-configuration.png) <br /> ![Wiring Configuration 2](../../images/wiring-configuration-2.jpg)
- Connect the white and blue wires to the micro controller.  The ATM is upside down in this picture - the white wire goes 6 holes in, and the blue wire is the last hole. <br /> ![Wiring Configuration 3](../../images/wiring-configuration-3.jpg).
- When you are all finished wiring, it should look like this! <br /> ![Wiring Configuration 4](../../images/wiring-configuration-4.jpg)

#### Connecting it to power
- Prepare the battery bank. Place shrink tubing over the red and black wires and then solder the red wire to the wire coming from the center toggle switch wire.  Connect a black wire to the breadboard and solder that to the battery bank black wire. <br /> ![Battery 1](../../images/battery-1.jpg)
- Using a lighter, shrink the shrink tubing. <br /> ![Battery 2](../../images/battery-2.jpg)
- Place a 3M strip to the side of the battery bank and press it against the front so we have plenty of room for our coins to drop into the ATM. ![Battery 3](../../images/battery-3.jpg)

#### Slide the Panel on @TODO
- Time to close this thing up and power it on! <br /> ![Final](../../images/led-final-1.jpg)
- Here is our finished product.  Power it on and enjoy! <br /> ![Final 2](../../images/led-final-2.jpg) 