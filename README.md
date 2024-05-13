Ohjelma on luotu syksyllä 2020 Tampereen yliopiston kurssilla Ohjelmointi 2 - Rakenteet

Käyttöohjeet ja lyhyt kuvaus projektissa fruit_flip
luotuun peliin match3

Pelin tarkoitus on jäljitellä myös netistä löytyvää tietokonepeliä
match3. Ohjelman suorituksen jälkeen näytölle avautuu käyttöliittymä,
jossa on värikkäistä neliöistä koostuvat pelilauta, painonappi vaihdon
suunnalle, numerovalikot rivin ja sarakkeen valintaan, GO-nappula, jota
painamalla siirto tapahtuu, painonappi laudan alustusta varten,
radiopainikkeet viiveen valinnalle, valintalaatikko sille, täytetäänkö
tyhjät kohdat laudalta vai ei, sekä virheilmoituksille tarkoitettu
tekstikenttä. Tässä toteutuksessa värikkäät neliöt kuvastavat alkuperäisessä
pelissä esintyviä hedelmiä.

Pelin säännöt lyhyesti:

Pelaaja voi käyttöliittymässä valita rivin ja sarakkeen, jotka ovat hänen
haluamansa hedelmän koordinaatit. Tämä hedelmä vaihtaa paikkaa joko sen oikealla
puolella tai alapuolella olevan hedelmän kanssa, riippuen siirron suunnasta.
Siirron suuntaa voi vaihtaa ylimpänä olevalla painonapilla. Painamalla paini-
ketta silloin kun siinä lukee Horizontal, käyttäjä valitsee horisontaalisen
eli vaakasuuntaisen suunna. Samoin painamalla nappia silloin, kun siinä lukee
vertical, pelaaja valitsee pystysuuntaisen siirron.

Jos siirron jälkeen laudalta löytyy joko vaakasuuntainen tai pystysuuntainen
match, nämä hedelmät poistetaan ja niiden yläpuolella olevat hedelmät tippuvat
alaspäin, kunnes laudalla neliöiden keskellä ei ole enää tyhjiä kohtia. match
tarkoittaa tässä pelissä sitä, että samalla rivillä tai samassa sarakkeessa
on kolme tai enemmän samanväristä hedelmää vierekkäin tai päällekkäin.

Tässä versiossa ei ole pisteenlaskua, joten kyseessä ei ole varsinaisesti peli,
vaan ohjelma, joka mallintaa yksinkertaista versiota alkuperäisestä pelistä.

Koodiin on tehty lisäominaisuudet 1, 2 ja 8:

1. Käyttäjällä on Fill Board - valintapainiketta painamalla mahdollisuus valita
täytetäänkö pelilaudan yläosa hedelmillä sitä mukaa, kun niitä tippuu alaspäin.
Tässä ominaisuudessa on joitakin puutteita, sillä se toimii hiukan vaihtelevasti.
Ajan puutteen vuoksi en saanut ominaisuutta täysin toimimaan, mutta jätin sen
koodiin mahdollisten osapisteiden varalta.

2. Käyttäjällä on mahdollisuus valita onko hedelmien poistamisen ja pudotuksen
välissä viivettä vai ei. Tässä koodissa viive on lisätty myös putoamisen ja
poistamisen välille kohtaan, jossa putoamisen seurauksena syntyy uusia matcheja
Tämä on tehty pelin selkeyden parantamiseksi, vaikka tätä ei tehtävänannossa suoraan
vaadittukkaan.

8. Peliasetelman voi palauttaa milloin vain alkutilanteeseen, jolloin pelilaudalle
arpoutuu uudet hedelmät ja osa painikkeista ja valintanappuloista alustetaan.

Virhetilanteita ja rajatapauksia:

Pelissä käyttäjä voi päästä ainoastaan yhteen virheelliseen tilanteeseen. Jos
käyttäjä yrittää vaihtaa tyhjää kohtaa, annetaan virheilmoitus, eikä mitään
siirtoa tehdä. Ohi-indeksointi on tädä toteutuksessa kuitattu siten, että
sarakkeen ja rivinääriarvot muuttuvat sen mukaan onko valittuna pysty- vai
vaakasuuntainen siirto. Pienin arvo molemmissa on yksi. Jos tehdään vaakasuuntainen
siirto, sarakkeelle ei voi antaa suurempaa arvoa kuin ROWS-1, jotta hedelmää
ei yritetä vaihtaa laudan ulkopuolella olevan kanssa. Samoin on menetelty
rivien maksimiarvon kanssa pystysuuntaisessa siirrossa.

Lisäksi GO - nappulaa ei voi painaa, ellei kertaakaan ole muutettu sekä rivin,
että sarakkeen arvoa.

Viiveen arvoa voi muuttaa sekä ennen pelin aloitusta, että pelin aikana.
Myös muut radiopainikkeet (GO-nappulaa lukuunottamatta) ovat aina käytössä.

Pelin toteutusratkasuja:

Peli on toteutettu lyhyesti siten, että ensin käyttäjän on annettava jonkin
neliön koordinaatit. Sen jälkeen kun hän painaa GO-painiketta, tehdään siirto
suunnasta riippuen. GO-nappulan slotissa kutsutaan check-slotteja sekä
vaakasuuntaiseelle, että pystysuuntaiselle tarkistukselle. Jos match lötyy,
näissä funktioissa kutsutaan remove-metodia, joka poistaa samanväriset vierekkäiset,
tai pystusuuntaiset neliöt. Remove-metodissa puolestaan kutsutaan drop-funktiota
(pystysuuntaisella  ja vaakasuuntaiseelle omansa), jossa tapahtuu varsinainen
pudotus.

Kaikki pelilaudalla olevat neliöt on pelilaudan luomisvaiheessa talletettu
vektoriin, jonka alkiot ovat sarakevektoreita. Sarakevektoreissa on peräkkäin
neliöt kullekin riville. Kun pelilaudalta poistetaa jotain, se asetetaan
nollapointteriksi.

Luokan attribuutteina on muunmuassa neliön koot ja rivien ja sarakkeiden määrä.
Näitä voi luokan määrittelyssä muokata haluamikseen.
