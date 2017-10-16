# stream-encipherment
Information theory, lab #2

1\. Implement stream encryption/decryption system for files of any type, using the LFSR-based key sequence generator. Initial register state should be specified by user. Text field of initial register state should ignore any characters except 1 and 0. Generated key (sequence of 1's and 0's) and encrypted file in binary representation should be displayed on the screen. Program should not be a console application. Process result is an encrypted/decrypted file.

\* Implement stream encryption/decryption system for files of any type, using Geffe algorithm with 3 registers LFSR1, LFSR2 and LFSR3. The initial state of registers should be specified by user. Text field of initial register state should ignore any characters except 1 and 0. Generated Geffe key, three intermediate keys (LFSR1, LFSR2 and LFSR3) and encrypted file in binary representation should be displayed on the screen. Process result is an encrypted/decrypted file. Implement as an extension of program #1.

\** Implement stream encryption/decryption algorithm RC4 for files of any type. The initial state of registers should be specified by user. Initial key is a sequence of numbers in 0..255 range. Generated key in decimal representation should be displayed on the screen. Process reult is an encrypted/decrypted file. Implement as an extension of program #1.

Degree of a polynomial:<br>
| LFSR1 | 25 |<br>
| LFSR2 | 33 |<br>
| LFSR3 | 23 |<br>
