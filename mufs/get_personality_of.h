/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2013                                    */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under either the GNU General   */
/* Public License (GPL) version 2 or the Eclipse Public License     */
/* (EPL) at your discretion.                                        */
/*                                                                  */
/* ================================================================ */

#include <linux/of.h>
#include "platforms/bgq/personality.h"

static struct bgq_personality * get_bgpersonality() {
    struct device_node *dn;
    const u64 *prop;
    int pers_sz;

    dn = of_find_compatible_node(NULL, NULL, "ibm,bgq-soc");
    if (!dn) {
        pr_info("%s: No BG/Q node\n", __func__);
        return NULL;
    }

    prop = of_get_property(dn, "ibm,bgq-personality", &pers_sz);
    if (!prop) {
        pr_emerg("%s: No BG/Q personality\n", __func__);
        return NULL;
    }
    if (pers_sz != sizeof(struct bgq_personality)) {
        pr_emerg("%s: BG/Q personality bad size, got %d, expect %ld\n",
             __func__, pers_sz, sizeof(struct bgq_personality));
        return NULL;
    }

    return (struct bgq_personality *) prop;
}


