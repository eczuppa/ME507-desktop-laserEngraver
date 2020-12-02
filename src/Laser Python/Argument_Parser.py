
import argparse


class ArgumentParser(object):
    def __init__(self, parser_description, parser_usage):
        self.parser = argparse.ArgumentParser(
                                              description=parser_description,
                                              formatter_class=argparse.RawTextHelpFormatter,
                                              epilog=parser_usage,
                                              add_help=False)

        locations_group = self.parser.add_argument_group('required arguments')
        optional_group = self.parser.add_argument_group('optional arguments')
                
        locations_group.add_argument(
            '[no deliminator]',
            help = 'Input Gcode File Path')
                            
        optional_group.add_argument(
            '-h', '--help',
            action='help',
            help='show this help message and exit')


    def getArgs(self):
        return self.parser.parse_args()
    
    
    def printHelp(self):
        self.parser.print_help()


#--------------------------------------------------------------------------------------------
#--------------------------------------------------------------------------------------------

def createParserInstructions():
    """ help and usage message creation """
    title = 'This code converts Gcode.\n'
    usage = 'Example usage:\n'
    usage += '  {0:30} ./python Optimize_Gcode.py "[gcodefilepath]"\n'.format('Convert Gcode to line array:')

    
    return title, usage
