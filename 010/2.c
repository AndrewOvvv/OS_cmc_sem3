#include <stdint.h>
#include <stdlib.h>


struct RandomGenerator;


typedef struct RandomOperations
{
    void (*destroy)(struct RandomGenerator *rr);
    int32_t (*next)(struct RandomGenerator *rr);
} RandomOperations;


typedef struct RandomGenerator
{
    int64_t base;
    int64_t multiplier;
    int64_t increment;
    int64_t MOD;
    const RandomOperations *ops;
} RandomGenerator;

    
void
destroy(struct RandomGenerator *rr)
{
    free(rr);
}


int32_t
next(struct RandomGenerator *rr)
{
    rr->base = (rr->multiplier * rr->base + rr->increment) % rr->MOD;
    return rr->base;
}


const RandomOperations RND_OPR = {destroy, next};


RandomGenerator *
random_create(int32_t seed)
{
    RandomGenerator *cur = malloc(sizeof(*cur));
    cur->ops = &RND_OPR;
    cur->base = seed;
    cur->multiplier = 1103515245;
    cur->increment = 12345;
    cur->MOD = (1ll << 31);
    return cur;
}
