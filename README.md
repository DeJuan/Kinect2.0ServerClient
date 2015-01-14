This is a work-in-progress for transmission of Kinect for Windows 2.0 depth images from one computer running the Kinect
to another computer without the Kinect. Currently at Stage 1 of 5:

Stage 1: Write the client server code for transmission and reception of character buffers,
using UDP as the protocol. 

Stage 2: Confirm that the code can transmit simple messages and debug

Step 3: Change the code so it serializes and transmits the Kinect depth image

Step 4: Add deserialization on the client side to reconstruct the image. 

Step 5: Debug the process.