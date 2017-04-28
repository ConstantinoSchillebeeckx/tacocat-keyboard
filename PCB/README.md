# PCB

The PCB was designed to be symmetric so that the same half could be used for both the left and the right side of the keyboard; the PCB simply needs to be flipped over depending on the side that is used.

Features:
- two symmetric keyboard halves
- wireless or powered operation
- wifi & bluetooth connected
- can be used as two halves with a TRRS audio cable or as single joined keyboard (no cable needed)
- audio passthrough

The board can be ordered from OshPark by clicking the image below.

<a href="https://oshpark.com/shared_projects/jsQ9hCTr"><img src="https://oshpark.com/assets/badge-5b7ec47045b78aef6eb9d83b3bac6b1920de805e9a0c227658eac6e19a045b9c.png" alt="Order from OSH Park"></img></a>

## Notes

To populate, place all Cherry MX switches, LEDs, diodes, capacitors and resistors on the *top* side of the board; the ESP32 and all IO jacks are placed on the *bottom* of the PCB.

Also:
- the top TRRS jack may hit one of the alignment pins of the switch; this pins may have to be manually cut
- the male I2C connection has plastic alignment pins that will only fit into the PCB after the two existing holes (used by the female end) are drilled out to a diamter of 1mm

## BOM

The items listed below are for two keyboard halves; an [online BOM](https://www.digikey.com/classic/RegisteredUser/BOMBillOfMaterials.aspx?path=1&exist=1&id=5155922) has also been generated for ease of purchase.

**NOTE:** the BOM listed includes various USB & TRRS jacks needed to build custom cables for the project, however the list does not include the wires needed to build the cables. You'll need to source a 5 conductor cable to connect the keyboard halves and an 8 conductor cable (4 for USB, 4 for TRRS) for the cable connecting the keyboard to the computer.


## Layout

#### Circuit schematic

<img src="https://github.com/ConstantinoSchillebeeckx/tacocat-keyboard/raw/master/assets/img/circuit_layout.png" width=500 />

#### PCB top

<img src="https://github.com/ConstantinoSchillebeeckx/tacocat-keyboard/raw/master/assets/img/pcb_top.png" width=500 />

#### PCB bottom

<img src="https://github.com/ConstantinoSchillebeeckx/tacocat-keyboard/raw/master/assets/img/pcb_top.png" width=500 />
