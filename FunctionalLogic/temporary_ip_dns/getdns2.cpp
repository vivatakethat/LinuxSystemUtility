





main(argc, argv) int argc; char *argv[]; { char *nsList[MAX_NS]; /* list of name servers */ int nsNum = 0; /* number of name servers in list */ /* sanity check: one (and only one) argument? */ if(argc != 2){ (void) fprintf(stderr, "usage: %s zone\n", argv[0]); exit(1); } (void) res_init( ); /* * Find the name servers for the zone. * The name servers are written into nsList. */ findNameServers(argv[1], nsList, &nsNum); /* * Query each name server for the zone's SOA record. * The name servers are read from nsList. */ queryNameServers(argv[1], nsList, nsNum); exit(0); } docstore.mik.ua/orelly/networking_2ndEd/dns/ch15_02.htm