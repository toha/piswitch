#include "tx.h"

void tx_data (protocol_t * p)
{
    switch (p->type)
    {
        case PROTOCOL1:
			tx_data_protocol1(&p->p1);
            break;
        case PROTOCOL2:
			tx_data_protocol2(&p->p2);
            break;
        case PROTOCOL3:
			tx_data_protocol3(&p->p3);
            break;
        case PROTOCOL4:
			tx_data_protocol4(&p->p4);
            break;
        case PROTOCOL5:
			tx_data_protocol5(&p->p5);
            break;
        default:
						printf("%d\n", p->type);
						printf("%d\n", PROTOCOL5);
            printf("ERRRRRROR\n");
            break;
    }
}
