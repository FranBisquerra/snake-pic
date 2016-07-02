/******************************************************************************/
/*                                                                            */
/*  Description: Uart library (header)                                       */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

void UARTInit();
void UARTSendString(unsigned char *data);
void UARTSendInt(unsigned int data);
char UARTRecv();