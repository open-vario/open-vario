# -*- coding: utf-8 -*-

import sys

from common.ov_flightfile import load_flight

# Entry point
if __name__ == '__main__':

    exit_code = 1

    print("######################################")
    print("    OpenVario flight file converter")
    print("######################################")
    print("")

    # Check args
    if (len(sys.argv) > 1):

        # Open input file
        print("Converting flight '{}'...".format(sys.argv[1]))
        flight_file = load_flight(sys.argv[1])
        if not flight_file is None:

            # Open output KML file
            kml_file = sys.argv[1] + ".kml"
            if (len(sys.argv) > 2):
                kml_file = sys.argv[2]
            try:
                file = open(kml_file, "w")
                print("Saving flight '{}'...".format(kml_file))

                # Write header
                file.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
                file.write("<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n")
                file.write("\t<Document>\n")
                file.write("\t\t<name>Flight with glider : {} - {}</name>\n".format(
                    flight_file.header.glider, flight_file.header.timestamp))
                file.write("\t\t<Style id=\"redline\">\n")
                file.write(
                    "\t\t\t<LineStyle><color>7F0000FF</color><width>6</width></LineStyle>\n")
                file.write("\t\t</Style>\n")

                # Takeoff
                takeoff = flight_file.entries[0]
                file.write("\t\t<Placemark>\n")
                file.write("\t\t\t<name>Takeoff</name>\n")
                file.write("\t\t\t<Point>\n")
                file.write("\t\t\t\t<coordinates>{:.6f},{:.6f},{}</coordinates>\n".format(
                    takeoff.gnss.longitude, takeoff.gnss.latitude, int(takeoff.gnss.altitude / 10)))
                file.write("\t\t\t</Point>\n")
                file.write("\t\t</Placemark>\n")

                # Landing
                landing = flight_file.entries[len(flight_file.entries) - 1]
                file.write("\t\t<Placemark>\n")
                file.write("\t\t\t<name>Landing</name>\n")
                file.write("\t\t\t<Point>\n")
                file.write("\t\t\t\t<coordinates>{:.6f},{:.6f},{}</coordinates>\n".format(
                    landing.gnss.longitude, landing.gnss.latitude, int(landing.gnss.altitude / 10)))
                file.write("\t\t\t</Point>\n")
                file.write("\t\t</Placemark>\n")

                # Trace
                file.write("\t\t<Placemark>\n")
                file.write("\t\t\t<styleUrl>#redline</styleUrl>\n")
                file.write("\t\t\t<LineString>\n")
                file.write("\t\t\t\t<altitudeMode>absolute</altitudeMode>\n")
                file.write("\t\t\t\t<coordinates>\n")
                for entry in flight_file.entries:
                    file.write("\t\t\t\t\t{:.6f},{:.6f},{}\n".format(
                        entry.gnss.longitude, entry.gnss.latitude, int(entry.gnss.altitude / 10)))
                file.write("\t\t\t\t</coordinates>\n")
                file.write("\t\t\t</LineString>\n")
                file.write("\t\t</Placemark>\n")

                # Write footer
                file.write("\t</Document>\n")
                file.write("</kml>")

                file.close()

                exit_code = 0

            except:
                print("Unable to open output file : {}".format(kml_file))
    else:
        print("Missing file to convert as input parameter")

    print("Done!")

    sys.exit(exit_code)
