import modul as md
import numpy as np

konstante = md.Konstante()

tijelo1 = md.Tijelo(
    np.array((384400e3, 0)),
    np.array((0, 1022)),
    konstante.M_s
)

tijelo2 = md.Tijelo(
    np.array((0, 0)),
    np.array((0, 0)),
    konstante.M_z
)

gibanje = md.Gibanje(tijelo1, tijelo2, konstante.godina, 3600*24)
gibanje.plot()