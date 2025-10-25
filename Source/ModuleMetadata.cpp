/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include <Nirvana/ModuleMetadata.h>
#include <Nirvana/platform.h>
#include <Nirvana/Main.h>
#include <Nirvana/ModuleInit.h>
#include <Nirvana/SemVer.h>
#include <unordered_set>

namespace Nirvana {

bool ModuleMetadata::check () noexcept
{
	if (ModuleType::MODULE_UNKNOWN == type) {
		const ModuleMetadataEntry* startup = nullptr;
		for (const auto& entry : entries) {
			if (entry.command == OLF_MODULE_STARTUP || entry.command == OLF_PROCESS_STARTUP) {
				if (startup) {
					set_error ("Duplicated startup entry");
					return false;
				} else
					startup = &entry;
			}
		}

		if (!startup) {
			set_error ("Startup entry missing");
			return false;
		} else if (startup->command == OLF_PROCESS_STARTUP) {
			if (CORBA::Internal::RepId::compatible (startup->interface_id, CORBA::Internal::RepIdOf <Nirvana::Main>::id))
				type = ModuleType::MODULE_EXECUTABLE;
			else {
				set_error ("Unknown process startup interface: " + startup->interface_id);
				return false;
			}
		} else if (CORBA::Internal::RepId::compatible (startup->interface_id, CORBA::Internal::RepIdOf <Nirvana::ModuleInit>::id)) {
			if (startup->flags & OLF_MODULE_SINGLETON)
				type = ModuleType::MODULE_SINGLETON;
			else
				type = ModuleType::MODULE_CLASS_LIBRARY;
			if (!SemVer ().parse (startup->name)) {
				set_error ("Invalid module name: " + startup->name);
				return false;
			}
			name = startup->name;
		} else {
			set_error ("Unknown startup interface: " + startup->interface_id);
			return false;
		}

		switch (type) {
		case ModuleType::MODULE_EXECUTABLE:
			for (const auto& entry : entries) {
				switch (entry.command) {
				case OLF_EXPORT_INTERFACE:
				case OLF_EXPORT_OBJECT:
				case OLF_EXPORT_LOCAL:
					set_error ("Executable can not export objects");
					return false;
					break;

				case OLF_IMPORT_INTERFACE:
					if (CORBA::Internal::RepId::compatible (entry.interface_id, CORBA::Internal::RepIdOf <CORBA::Internal::ObjectFactory>::id)) {
						set_error ("Process can not import ObjectFactory interface");
						return false;
					}
					break;
				}
			}
			break;

		case ModuleType::MODULE_SINGLETON:
			for (const auto& entry : entries) {
				if (OLF_EXPORT_INTERFACE == entry.command) {
					set_error ("Singleton can not export pseudo interfaces");
					return false;
				}
			}
			break;
		}

		std::unordered_set <std::string> names;
		for (const auto& entry : entries) {
			switch (entry.command) {
			case OLF_EXPORT_OBJECT:
				if (!CORBA::Internal::RepId::compatible (entry.interface_id, CORBA::Internal::RepIdOf <PortableServer::ServantBase>::id)) {
					set_error ("Invalid ServantBase interface: " + entry.interface_id);
					return false;
				}
				break;
			case OLF_EXPORT_LOCAL:
				if (!CORBA::Internal::RepId::compatible (entry.interface_id, CORBA::Internal::RepIdOf <CORBA::LocalObject>::id)) {
					set_error ("Invalid LocalObject interface: " + entry.interface_id);
					return false;
				}
				break;
			}

			switch (entry.command) {
			case OLF_EXPORT_INTERFACE:
			case OLF_EXPORT_OBJECT:
			case OLF_EXPORT_LOCAL:
				if (!names.insert (entry.name).second) {
					set_error ("Duplicated export name: " + entry.name);
					return false;
				}
				break;
			}
		}
	}

	return ModuleType::MODULE_ERROR != type;
}

void ModuleMetadata::print (std::ostream& out)
{
	check ();

	if (ModuleType::MODULE_ERROR == type) {
		out << "Error: " << error << std::endl;
		return;
	}

	struct ImpEx
	{
		std::string name;
		std::string interface_id;

		bool operator < (const ImpEx& rhs) const
		{
			return name < rhs.name;
		}
	};

	std::vector <ImpEx> imports, exp_interfaces;
	std::vector <std::string> exp_objects, exp_local;

	for (const auto& entry : entries) {
		switch (entry.command) {
		case OLF_IMPORT_INTERFACE:
		case OLF_IMPORT_OBJECT:
			imports.push_back ({ entry.name, entry.interface_id });
			break;
		case OLF_EXPORT_INTERFACE:
			exp_interfaces.push_back ({ entry.name, entry.interface_id });
			break;
		case OLF_EXPORT_OBJECT:
			exp_objects.push_back (entry.name);
			break;
		case OLF_EXPORT_LOCAL:
			exp_objects.push_back (entry.name);
			break;
		}
	}

	std::sort (imports.begin (), imports.end ());
	std::sort (exp_interfaces.begin (), exp_interfaces.end ());
	std::sort (exp_objects.begin (), exp_objects.end ());
	std::sort (exp_local.begin (), exp_local.end ());

	const char* platform_name = "Unknown platform";
	switch (platform) {
	case PLATFORM_X86:
		platform_name = "Intel 386";
		break;
	case PLATFORM_X64:
		platform_name = "AMD64";
		break;
	case PLATFORM_ARM:
		platform_name = "ARM Little-Endian";
		break;
	case PLATFORM_ARM64:
		platform_name = "ARM64 Little-Endian";
		break;
	}

	out << "Platform: " << platform_name << std::endl;

	const char* mod_type = "Class library";
	switch (type) {
	case ModuleType::MODULE_SINGLETON:
		mod_type = "Singleton";
		break;
	case ModuleType::MODULE_EXECUTABLE:
		mod_type = "Executable";
		break;
	case ModuleType::MODULE_NIRVANA:
		mod_type = "Native Executable";
		break;
	}

	out << "Type: " << mod_type << std::endl;
	if (!name.empty ())
		out << "Name: " << name << std::endl;

	if (!imports.empty ()) {
		out << "IMPORTS: " << imports.size () << std::endl;
		for (const auto& rec : imports) {
			out << '\t' << rec.name << '\t' << rec.interface_id << std::endl;
		}
	}
	if (!exp_interfaces.empty ()) {
		out << "EXPORT INTERFACES: " << exp_interfaces.size () << std::endl;
		for (const auto& rec : exp_interfaces) {
			out << '\t' << rec.name << '\t' << rec.interface_id << std::endl;
		}
	}
	if (!exp_objects.empty ()) {
		out << "EXPORT OBJECTS: " << exp_objects.size () << std::endl;
		for (const auto& rec : exp_objects) {
			out << '\t' << rec << std::endl;
		}
	}
	if (!exp_local.empty ()) {
		out << "EXPORT LOCAL OBJECTS: " << exp_local.size () << std::endl;
		for (const auto& rec : exp_local) {
			out << '\t' << rec << std::endl;
		}
	}
}

}
