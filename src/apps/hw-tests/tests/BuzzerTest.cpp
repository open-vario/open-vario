/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "BuzzerTest.h"
#include "IOs.h"



namespace open_vario
{

/** \brief Constructor */
BuzzerTest::BuzzerTest(IBuzzer& buzzer)
: HwTestBase("Buzzer", m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_buzzer(buzzer)
, m_menu_entries()
{
    MENU_ENTRY(0, "Probe", BuzzerTest, probe);
    MENU_ENTRY(1, "Turn on", BuzzerTest, on);
    MENU_ENTRY(2, "Turn off", BuzzerTest, off);
    MENU_ENTRY(3, "Song", BuzzerTest, song);
}

/** \brief Probe test */
void BuzzerTest::probe(const size_t entry, Console& console)
{
    console.writeLine("Probing buzzer...");
    if (m_buzzer.configure())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }    
}

/** \brief On test */
void BuzzerTest::on(const size_t entry, Console& console)
{
    console.write("Frequency in Hz : ");
    uint32_t frequency = console.readUint();

    console.write("Volume in % : ");
    uint8_t volume = console.readUint();
    if (m_buzzer.setVolume(volume))
    {
        console.writeLine("Turning on buzzer...");
        if (m_buzzer.play(frequency))
        {
            console.writeLine("Success!");
        }
        else
        {
            console.writeLine("Failed!");
        }
    }
    else
    {
        console.writeLine("Failed!");
    }
}

/** \brief Off test */
void BuzzerTest::off(const size_t entry, Console& console)
{
    console.writeLine("Turning off buzzer...");
    if (m_buzzer.stop())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }
}

/** \brief Song test */
void BuzzerTest::song(const size_t entry, Console& console)
{
    enum Notes { DO = 0, DO_S, RE, MI_F, MI, FA, FA_S, SOL, SOL_S, LA, SI_F, SI };
    enum Tempo { B = 1u, M = 2u, C = 4u, Q = 8u, SQ = 16u, DQ = 32u };
    static const uint32_t NOTES[] = { 523u, 554u, 587u, 622u, 659u, 698u, 740u, 784u, 831u, 880u, 932u, 988u };
    struct Note
    {
        Notes note;
        uint8_t octave;
        Tempo tempo;
    };
    static const Note song[] = {
                                    { DO, 2u, Q},
                                    { DO, 2u, Q},
                                    { RE, 2u, Q},
                                    { MI, 2u, Q},
                                    { DO, 2u, Q},
                                    { MI, 2u, Q},
                                    { RE, 2u, Q},
                                    { SOL, 1u, Q},
                                    { DO, 2u, Q},
                                    { DO, 2u, Q},
                                    { RE, 2u, Q},
                                    { MI, 2u, Q},
                                    { DO, 2u, C},
                                    { SI, 1u, Q},
                                    { SOL, 1u, Q},
                                    { DO, 2u, Q},
                                    { DO, 2u, Q},
                                    { RE, 2u, Q},
                                    { MI, 2u, Q},
                                    { FA, 2u, Q},
                                    { MI, 2u, Q},
                                    { RE, 2u, Q},
                                    { DO, 2u, Q},
                                    { SI, 1u, Q},
                                    { SOL, 1u, Q},
                                    { LA, 1u, Q},
                                    { SI, 1u, Q},
                                    { DO, 2u, M},

                                    { LA, 1u, C},
                                    { SI, 1u, Q},
                                    { LA, 1u, Q},
                                    { SOL, 1u, Q},
                                    { LA, 1u, Q},
                                    { SI, 1u, Q},
                                    { DO, 2u, C},
                                    { SOL, 1u, C},
                                    { LA, 1u, Q},
                                    { SOL, 1u, Q},
                                    { FA, 1u, Q},
                                    { MI, 1u, C},
                                    { SOL, 1u, C},
                                    { LA, 1u, C},
                                    { SI, 1u, Q},
                                    { LA, 1u, Q},
                                    { SOL, 1u, Q},
                                    { LA, 1u, Q},
                                    { SI, 1u, Q},
                                    { DO, 2u, Q},
                                    { SOL, 1u, Q},
                                    { SI, 1u, Q},
                                    { RE, 2u, Q},
                                    { DO, 2u, C}
                                };
    static const size_t SONG_SIZE = sizeof(song) / sizeof(Note);
    
    console.write("Tempo (duration of quaver in ms) : ");
    uint32_t tempo = console.readUint() * Q;

    console.write("Volume in % : ");
    uint8_t volume = console.readUint();
    m_buzzer.setVolume(volume);

    console.writeLine("Playing song (press any key to stop)...");
    char c = 0;
    bool end = false;
    while (!end)
    {
        for (size_t i = 0; (i < SONG_SIZE) && !end; i++)
        {
            const Note& note = song[i];
            m_buzzer.play(NOTES[note.note] * note.octave);
            IOs::getInstance().waitMs(tempo / note.tempo);
            end = console.peekChar(c);
        }
    }

    m_buzzer.stop();
}

}
