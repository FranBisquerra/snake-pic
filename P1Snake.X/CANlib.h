/******************************************************************************/
/*                                                                            */
/*  Description: CAN library (header)                                         */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

void CANInit(unsigned char mode);

void CANsendMessage(unsigned int sid, char * data, unsigned char size);