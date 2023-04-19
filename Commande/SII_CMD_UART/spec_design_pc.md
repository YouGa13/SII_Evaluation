# DESIGN - TRAMES

- Morse  Mini-Project (Board Side)
- STM32 Nucleo-F103RBT6
- V1.0

# TABLE DES MATIERES
- [DESIGN - TRAMES](#design---trames)
- [TABLE DES MATIERES](#table-des-matieres)
- [Type SII\_TRAMES\_Trame](#type-sii_cmd_trame)
- [Function SII\_TRAMES\_Create_Trame()](#function-sii_cmd_create_trame())
- [Function SII\_TRAMES\_Init_Trame()](#function-sii_cmd_init_trame())
- [Function SII\_TRAMES\_Funct_ToSendData_UART()](#function-sii_cmd_init_trame())


# Type SII_TRAME_Trame
- Ref : e001 (Specification.md)
- Input :
- Output :
```
SII_TRAME_Trame = { loop, iteration, timers_table, data_lenght,data,stop }
```
# Function SII_CMD_CreateTrame()
- Ref : e002 (Specification.md)
- Input : `loop, iteration, timers, data_lenght, data, stop`
- Output : `SII_TRAME_Trame SII_TRAME_ToSend` 
```
if loop, iteration, timers_table, data_lenght, data, stop
|   return SII_TRAME_ToSend
```
# Function SII_TRAMES_InitTrame()
- Ref : e002 (Specification.md)
- Input : `sii_trames_trame`
- Output : 
```
if sii_trames_trame
|   sii_trames_trame.loop = 0;
|   sii_trames_trame.iteration = 0;
|   sii_trames_trame.timers_table[3] = {10, 50, 100};
|   sii_trames_trame.data_lenght;
|   sii_trames_trame.data;
|   sii_trames_trame.stop = \n;
```
# Function SII_TRAMES_Funct_ToSendData_UART()
- Ref : e002 (Specification.md)
- Input : `sii_trames_trame, serial_port`
- Output : 
```
if sii_trames_trame, serial_port
|   transmission message;

```
# DESIGN - COMMANDES - ERRORS

- Morse  Mini-Project (Board Side)
- STM32 Nucleo-F103RBT6
- V1.0

# TABLE DES MATIERES
- [DESIGN - COMMANDES - ERRORS](#design---commandes)
- [TABLE DES MATIERES](#table-des-matieres)
- [Function SII\_CMD\_Errors_Gestion()](#function-sii_errors_gestion)

# Function SII_CMD_Errors_Gestion()

- Ref : c001, c002, c003, c004, c005, c006(Specification.md)
- Input : `option`
- Output :  

```
if option 
|   switch option :
|       case b :
|           print option message errors
|   .....
```
```