// The predicates in the body and head of traditional Datalog rules are relations, 
// and we refer to them interchangeably as predicates, relations or tables. 
// Each relation has a primary key, which consists of a set of fields that uniquely identify 
// each tuple within the relation. In the absence of other information, the primary key 
// is the full set of fields in the relation.
//
// materialize(name, lifetime, size, primary keys)
// If the primary key is the empty set (), then the primary key is the full set of fields in the relation
materialize(tLink, infinity, infinity, keys(1,2)).
// Every 1 second, node "Src" will send out a ePing event to its neighbors, i.e. node "Next"
r1 ePing(@Next, Src):-
    periodic(@Src, E, M),
    tLink(@Src, Next),
    M == 1,
    E == "n".
r2 ePing(@Next, Src):-
    path(@Src, Next).
%% r1 ePing(@Next, Src):-
%% periodic(@Src, E, M),
%% tLink(@Src, Next),
%% M == 1.
// When a node receive a ePing event from its neighbor, it sent back a ePong message
r3 ePong(@Next, Src):-
    ePing(@Src, Next).
// When a node receive a ePong event from its neighbor, this Ping-Pong procedure is indicated as finished
r4 ePingPongFinish(@Src):-
    ePong(@Src, Next).
