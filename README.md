# stream-encipherment
Information theory, lab #2

1\. Implement stream encryption/decryption system for files of any type, using the __LFSR-based key sequence generator__. The initial register state should be specified by user. The text field of the initial register state should ignore any characters except 1 and 0. The generated key (a sequence of 1's and 0's) and the encrypted file in binary representation should be displayed on the screen. This program should not be a console application. The result of the process is the encrypted/decrypted file.

\* Implement stream encryption/decryption system for files of any type, using the __Geffe algorithm__ with 3 shift registers (LFSR1, LFSR2 and LFSR3). The initial states of registers should be specified by user. The text field of the initial register state should ignore any characters except 1 and 0. The generated Geffe key, three intermediate keys (LFSR1, LFSR2 and LFSR3) and the encrypted file in binary representation should be displayed on the screen. The result of the process is the encrypted/decrypted file. Implement as an extension of program #1.

\** Implement stream __RC4 encryption/decryption algorithm__ for files of any type. The initial state of registers should be specified by user. The initial key is a sequence of numbers in 0..255 range. The generated key in decimal representation should be displayed on the screen. The result of the process is the encrypted/decrypted file. Implement as an extension of program #1.

__Degree of a polynomial:__<br>
| LFSR1 | 25 |<br>
| LFSR2 | 33 |<br>
| LFSR3 | 23 |<br>
