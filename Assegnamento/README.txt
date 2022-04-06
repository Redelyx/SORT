######################################################
# Assegnamento di Sistemi Operativi e in Tempo Reale #
#           Alice Cipriani mat. 340403               #
######################################################

Per prima cosa avviare il server con
        ./server

In seguito avviare l'hub con il paramentro j
        ./hub <parametro_j>

Quindi si possono avviare gli attuatori specificando la modalità: 0 per disinscrizione, 1(o qualsiasi altro valore) per iscriversi
        ./actuator <nome_attuatore> <modalità>
    Una volta avviato verrà richiesta la temperatura di Tgoal e successivamente i nomi dei sensori da ascoltare.
    i nomi dei sensori vanno inseriti uno alla volta premendo invio tra uno e l'altro. Per terminare l'inserimento 
    dei sensori scrivere "exit" e premere invio.

Si possono avviare i sensori specificando nome e numero di misure da rilevare
        ./sensor <nome_sensore> <numero_misure_da_rilevare>

