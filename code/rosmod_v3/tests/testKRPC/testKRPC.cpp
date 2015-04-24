#include <iostream>
#include <fstream>
#include <string>
#include "KRPC.pb.h"
using namespace std;

// This function fills in a Argument message based on user input.
void PromptForArgument(krpc::Argument* argument) {
  unsigned int position;
  cout << "Input position for argument: ";
  cin >> position;
  argument->set_position(position);
  cin.ignore(256, '\n');
  
  cout << "Input value for the argument (as a string): ";
  string value;
  getline(cin, value);
  argument->set_value(value);
}

// Main function:  Reads the entire address book from a file,
//   adds one argument based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0]  << " REQUEST_FILE" << endl;
    return -1;
  }

  krpc::Request request;

  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
      cout << "Enter service name: ";
      string service;
      getline(cin, service);
      request.set_service(service);

      cout << "Enter procedure name: ";
      string procedure;
      getline(cin, procedure);
      request.set_procedure(procedure);
    } else if (!request.ParseFromIstream(&input)) {
      cerr << "Failed to parse request." << endl;
      return -1;
    } else {
      cout << "Service name: " << request.service() << endl;
      cout << "Service procedure name: " << request.procedure() << endl;
      for (int i=0; i < request.arguments_size(); i++)
	{
	  const krpc::Argument& argument = request.arguments(i);
	  cout << "Argument position: " << argument.position() << endl;
	  cout << "Argument value: " << argument.value() << endl;
	}
    }
  }

  // Add an address.

  while (true)
    {
      cout << "Add argument (Y/N)? ";
      char result;
      cin >> result;
      if (result == 'Y')
	{
	  cin.ignore(256, '\n');
	  PromptForArgument(request.add_arguments());
	}
      else
	break;
    }

  {
    // Write the new request back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!request.SerializeToOstream(&output)) {
      cerr << "Failed to write request." << endl;
      return -1;
    }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
