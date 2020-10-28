/*
 * print.cc
 *
 * This module prints out the hardware tree depending on options passed
 * Output can be plain text (good for human beings), HTML (good for web brow-
 * sers), XML (good for programs) or "hardware path" (ioscan-like).
 *
 */

#include "print.h"
#include "options.h"
#include "version.h"
#include "osutils.h"
#include "config.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>


__ID("@(#) $Id$");

static unsigned int columns = 0, rows = 0;
  vector<st_HardWare>vec_HardWare;
static void init_wsize()
{
  struct winsize ws;
  char *env;

  if (!ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws))
  {
    rows = ws.ws_row;
    columns = ws.ws_col;
  }

  if (!rows)
  {
    env = getenv("LINES");
    if (env)
      rows = atoi(env);
    if (!rows)
      rows = 24;
  }

  if (!columns)
  {
    env = getenv("COLUMNS");
    if (env)
      columns = atoi(env);
    if (!columns)
      columns = 80;
  }
}


static void tab(int level,
bool connect = true)
{
  return;//takethat:放弃这些
  if (level <= 0)
    return;
  for (int i = 0; i < level - 1; i++)
    cout << "   ";
  cout << "  ";
  if (connect)
    cout << "*-";
  else
    cout << "  ";
}


bool desotry()
{
if(vec_HardWare.empty())
return true;
else
{
vec_HardWare.clear();
 return true;
}




}

 std::vector<st_HardWare> print(hwNode & node,
bool html,
int level)
{


  struct st_HardWare take_stHardWare;
  vector < string > config;
  vector < string > resources;
  if (html)
    config = node.getConfigValues("</td><td>=</td><td>");
  else
    config = node.getConfigValues("=");
  if (html)
    resources = node.getResources("</td><td>:</td><td>");
  else
    resources = node.getResources(":");

  if (html && (level == 0))
  {
    cout <<
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">" <<
      endl;
    cout << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << endl;
    cout << "<head>" << endl;
    cout << "<meta name=\"generator\" " <<
      " content=\"lshw-" << getpackageversion() << "\" />" << endl;
    cout << "<style type=\"text/css\">" << endl;
    cout << "  .first {font-weight: bold; margin-left: none; padding-right: 1em;vertical-align: top; }" << endl;
    cout << "  .second {padding-left: 1em; width: 100%; vertical-align: center; }" << endl;
    cout << "  .id {font-family: monospace;}" << endl;
    cout << "  .indented {margin-left: 2em; border-left: dotted thin #dde; padding-bottom: 1em; }" << endl;
    cout << "  .node {border: solid thin #ffcc66; padding: 1em; background: #ffffcc; }" << endl;
    cout << "  .node-unclaimed {border: dotted thin #c3c3c3; padding: 1em; background: #fafafa; color: red; }" << endl;
    cout << "  .node-disabled {border: solid thin #f55; padding: 1em; background: #fee; color: gray; }" << endl;
    cout << "</style>" << endl;

    cout << "<title>";
    cout << node.getId();//
    cout << "</title>" << endl;
    cout << "</head>" << endl;
    cout << "<body>" << endl;
  }

  if (visible(node.getClassName()))
  {
    tab(level, !html);

    if (html)
    {
      tab(level, false);
      cout << "<div class=\"indented\">" << endl;
      tab(level, false);
      cout << "<table width=\"100%\" class=\"node";
      if(!node.claimed())
        cout << "-unclaimed";
      else
      {
        if(node.disabled())
          cout << "-disabled";
      }
      cout << "\" summary=\"attributes of " << node.getId() << "\">" << endl;
    }

    if (html)
      cout << " <thead><tr><td class=\"first\">id:</td><td class=\"second\"><div class=\"id\">";
    //  take_stHardWare.id=node.getId();
    //cout << node.getId();//
    if (html)
      cout << "</div>";
    else
    {
      if (node.disabled())
        cout << _(" DISABLED");
      if (!node.claimed())
        cout << _(" UNCLAIMED");
    }
    if (html)
      cout << "</td></tr></thead>";
    cout << endl;

#if 0
    if (node.getHandle() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
      cout << "handle: ";
      if (html)
      {
        cout << "</td><td class=\"second\">";
        cout << escape(node.getHandle());
        cout << "</td></tr>";
      }
      else
        cout << node.getHandle()
      cout << endl;
    }
#endif

    if (html)
      cout << " <tbody>" << endl;

    if (node.getDescription() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
  //    cout << _("description") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\">";
        cout << escape(node.getDescription());
        cout << "</td></tr>";
      }
      else
    //  {
       // cout << node.getDescription();
       take_stHardWare.description= node.getDescription();
         take_stHardWare.id=node.getId();//take_add;
       //  cout<<"take_stHardWare.id="<<take_stHardWare.id<<endl;
    //  cout <<" take_stHardWare.description:"<< take_stHardWare.description;
   //   }
      //cout << endl;
    }

    if (node.getProduct() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
   //   cout << _("product") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\">";
        cout << escape(node.getProduct());
        cout << "</td></tr>";
      }
      else
     //   cout << node.getProduct();
        take_stHardWare.product=node.getProduct();
  //  cout<<"take_stHardWare.product:"<<take_stHardWare.product;
   //   cout << endl;
    }

    if (node.getVendor() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
   //   cout << _("vendor") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\">";
        cout << escape(node.getVendor());
        cout << "</td></tr>";
      }
      else
       // cout << node.getVendor();
        take_stHardWare.vendor=node.getVendor();
   //   cout<<"take_stHardWare.vendor:"<<take_stHardWare.vendor;
   //   cout << endl;
    }

    if (node.getPhysId() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
   //   cout << _("physical id") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\"><div class=\"id\">";
        cout << escape(node.getPhysId());
        cout << "</div></td></tr>";
      }
      else
       // cout << node.getPhysId();
         take_stHardWare.physid=node.getPhysId();
       //  cout<<" take_stHardWare.physid:"<< take_stHardWare.physid<<endl;
    //  cout << endl;
    }

    if (node.getBusInfo() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
   //   cout << _("bus info") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\"><div class=\"id\">";
        cout << escape(node.getBusInfo());
        cout << "</div></td></tr>";
      }
      else
       // cout << node.getBusInfo();
         take_stHardWare.businfo=node.getBusInfo();
  //  cout<<" take_stHardWare.businfo:"<<take_stHardWare.businfo;
     // cout << endl;
    }

    if (node.getLogicalName() != "")
    {
      vector<string> logicalnames = node.getLogicalNames();

      for(unsigned int i = 0; i<logicalnames.size(); i++)
      {
        tab(level + 1, false);
        if (html)
          cout << "<tr><td class=\"first\">";
      //  cout << _("logical name") << ": ";
        if (html)
        {
          cout << "</td><td class=\"second\"><div class=\"id\">";
          cout << escape(logicalnames[i]);
          cout << "</div></td></tr>";
        }
        else
         // cout << logicalnames[i];
           take_stHardWare.logicalnames.push_back(logicalnames[i]);
     //   cout << endl;
      }
    }

    if (node.getVersion() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
   //   cout << _("version") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\">";
        cout << escape(node.getVersion());
        cout << "</td></tr>";
      }
      else
  //      cout << node.getVersion();
         take_stHardWare.version= node.getVersion();
   //   cout << endl;
    }

    if (node.getDate() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
      cout << _("date") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\">";
        cout << escape(node.getDate());
        cout << "</td></tr>";
      }
      else
     //   cout << node.getDate();
         take_stHardWare.date=node.getDate();
      cout << endl;
    }

    if (node.getSerial() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
   //   cout << _("serial") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\">";
        cout << escape(enabled("output:sanitize")?REMOVED:node.getSerial());
        cout << "</td></tr>";
      }
      else
      //  cout << (enabled("output:sanitize")?REMOVED:node.getSerial());
    take_stHardWare.serial = (enabled("output:sanitize")?REMOVED:node.getSerial());
       
    //  cout << endl;
    }

    if (node.getSlot() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
      cout << _("slot") << ": ";
      if (html)
      {
        cout << "</td><td class=\"second\">";
        cout << escape(node.getSlot());
        cout << "</td></tr>";
      }
      else
       // cout << node.getSlot();
        take_stHardWare.slot=node.getSlot();
     // cout << endl;
    }

    if (node.getSize() > 0)
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
      cout << _("[size]") << ": ";
       //  take_stHardWare.size= kilobytes(node.getSize());
      if (html)
        cout << "</td><td class=\"second\">";
      switch (node.getClass())
      {
        case hw::disk:
          cout << kilobytes(node.getSize()) << " (" << decimalkilos(node.getSize()) << "B)";
            take_stHardWare.size= kilobytes(node.getSize());
          if (html)
            cout << "</td></tr>";
          break;
        case hw::display:
        case hw::memory:
        case hw::address:
        case hw::storage:
        case hw::volume:
        // cout << kilobytes(node.getSize());
        take_stHardWare.size= kilobytes(node.getSize());
          if (html)
            cout << "</td></tr>";
          break;

        case hw::processor:
        case hw::bus:
        case hw::system:
        //  cout << decimalkilos(node.getSize()) << "Hz";
          if (html)
            cout << "</td></tr>";
          break;

        case hw::network:
        //  cout << decimalkilos(node.getSize()) << "bit/s";
          if (html)
            cout << "</td></tr>";
          break;

        case hw::power:
          cout << node.getSize();
          cout << "mWh";
          if (html)
            cout << "</td></tr>";
          break;

        default:
          cout << node.getSize();
          if (html)
            cout << "</td></tr>";
      }
      cout << endl;
    }

    if (node.getCapacity() > 0)
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
     // cout << _("capacity") << ": ";
      if (html)
        cout << "</td><td class=\"second\">";
      switch (node.getClass())
      {
        case hw::disk:
          cout << kilobytes(node.getCapacity()) << " (" << decimalkilos(node.getCapacity()) << "B)";
          if (html)
            cout << "</td></tr>";
          break;
        case hw::memory:
        case hw::address:
        case hw::storage:
        case hw::volume:
     //    cout << kilobytes(node.getCapacity());//takethat:mark

          take_stHardWare.kilobytes=kilobytes(node.getCapacity());
          if (html)
            cout << "</td></tr>";
          break;

        case hw::processor:
        case hw::bus:
        case hw::system:
        {
        //  cout << decimalkilos(node.getCapacity());
        string st_decimalkilos=decimalkilos(node.getCapacity());
        st_decimalkilos.append("HZ");
           take_stHardWare.kilobytes=st_decimalkilos;
        //  cout << "Hz";
          if (html)
            cout << "</td></tr>";
          break;
        }
        case hw::network:
        {
        string decimalkilos_Network=decimalkilos(node.getCapacity());
        decimalkilos_Network.append( "bit/s");
        take_stHardWare.capacity= decimalkilos_Network;
       //   cout << decimalkilos(node.getCapacity());
      //    cout << "bit/s";
          if (html)
            cout << "</td></tr>";
          break;
        }
        case hw::power:
          cout << node.getCapacity();
          cout << "mWh";
          if (html)
            cout << "</td></tr>";
          break;

        default:
          cout << node.getCapacity();
          if (html)
            cout << "</td></tr>";
      }
      cout << endl;
    }

    if (node.getClass() == hw::address)
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
      if (node.getSize() == 0)
        cout << _("address") << ": " << hex << setfill('0') << setw(8) << node.
          getStart() << dec;
      else
        cout << _("range") << ": " << hex << setfill('0') << setw(8) << node.
          getStart() << " - " << hex << setfill('0') << setw(8) << node.
          getStart() + node.getSize() - 1 << dec;
      cout << endl;
    }

    if (node.getWidth() > 0)
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
     // cout << _("width") << ": ";
      if (html)
        cout << "</td><td class=\"second\">";
    //  cout << node.getWidth() << " bits";
       take_stHardWare.with=node.getWidth(); 
        
      if (html)
        cout << "</td></tr>";
    //  cout << endl;
    }

    if (node.getClock() > 0)
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
     // cout << _("clock") << ": ";
      if (html)
        cout << "</td><td class=\"second\">";
     // cout << decimalkilos(node.getClock());
     take_stHardWare.Clock =decimalkilos(node.getClock());
     // cout << "Hz";
      if (node.getClass() == hw::memory)
        cout << " (" << setiosflags(ios::fixed) << setprecision(1) << 1.0e9 / node.getClock() << "ns)";
      if (html)
        cout << "</td></tr>";
      cout << endl;
    }

    if (node.getCapabilities() != "")
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
     // cout << _("capabilities") << ": ";
      if (html)
      {
        vector < string > capabilities = node.getCapabilitiesList();
        cout << "</td><td class=\"second\">";
        for(unsigned i=0; i<capabilities.size(); i++)
        {
          cout << "<dfn title=\"" << escape(node.getCapabilityDescription(capabilities[i])) << "\">" << capabilities[i] << "</dfn> ";
        }
        cout << "</td></tr>";
      }
      else
     take_stHardWare.capabilities.push_back(node.getCapabilities());
      //  cout << node.getCapabilities();

     // cout << endl;
    }

    if (config.size() > 0)
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
  //    cout << _("configuration:");
      if (html)
        cout << "</td><td class=\"second\"><table summary=\"" << _("configuration of ") << node.
          getId() << "\">";
       string t_strconfig;
      for (unsigned int i = 0; i < config.size(); i++)
      {
        if (html)
          cout << "<tr><td class=\"sub-first\">";
        t_strconfig+=config[i].data();
      //take_stHardWare.config.push_back(config[i]);
      //  cout << " " << config[i];
        if (html)
          cout << "</td></tr>";
      }
  take_stHardWare.config=t_strconfig;



      if (html)
        cout << "</table></td></tr>";
    //  cout << endl;
    }

    if (resources.size() > 0)
    {
      tab(level + 1, false);
      if (html)
        cout << "<tr><td class=\"first\">";
   //   cout << _("resources:");
      if (html)
        cout << "</td><td class=\"second\"><table summary=\"" << _("resources of ") << node.
          getId() << "\">";
      for (unsigned int i = 0; i < resources.size(); i++)
      {
        if (html)
          cout << "<tr><td class=\"sub-first\">";
  take_stHardWare.resources.push_back(resources[i]);
     //   cout << " " << resources[i];
        if (html)
          cout << "</td></tr>";
      }
      if (html)
        cout << "</table></td></tr>";
      cout << endl;
    }

    if (html)
      cout << " </tbody>";

    if (html)
    {
      tab(level, false);
      cout << "</table></div>" << endl;
    }

  }                                               // if visible

//cout<<"line="<<__LINE__<<take_stHardWare.description<<endl;
vec_HardWare.push_back(take_stHardWare);
  for (unsigned int i = 0; i < node.countChildren(); i++)
  {
    if(html)
      cout << "<div class=\"indented\">" << endl;
    print(*node.getChild(i), html, visible(node.getClassName()) ? level + 1 : 1);
    if(html)
      cout << "</div>" << endl;
  }

  if (html)
  {
    if (visible(node.getClassName()))
    {
      tab(level, false);
    }
    if (level == 0)
    {
      cout << "</body>" << endl;
      cout << "</html>" << endl;
    }
  }
 

return vec_HardWare;
}


struct hwpath
{
  string path;
  string description;
  string devname;
  string classname;
};

static void printhwnode(hwNode & node, vector < hwpath > &l, string prefix = "")
{
  hwpath entry;

  entry.path = "";
  if (node.getPhysId() != "")
    entry.path = prefix + "/" + node.getPhysId();
  entry.description = node.asString();
  entry.devname = node.getLogicalName();
  entry.classname = node.getClassName();

  l.push_back(entry);
  for (unsigned int i = 0; i < node.countChildren(); i++)
  {
    printhwnode(*node.getChild(i), l, entry.path);
  }
}


static void printbusinfo(hwNode & node, vector < hwpath > &l)
{
  hwpath entry;

  entry.path = "";
  if (node.getBusInfo() != "")
    entry.path = node.getBusInfo();
  entry.description = node.asString();
  entry.devname = node.getLogicalName();
  entry.classname = node.getClassName();
  
  l.push_back(entry); 
  for (unsigned int i = 0; i < node.countChildren(); i++)
  { 
    printbusinfo(*node.getChild(i), l);
  }
}

static void printline(ostringstream & out)
{
  string s = out.str();

  if(isatty(STDOUT_FILENO) && (s.length()>columns))
    s.erase(columns);
  cout << s << endl;
  out.str("");
}


static void printincolumns( vector < hwpath > &l, const char *cols[])
{
  unsigned int l1 = strlen(_(cols[0])),
    l2 = strlen(_(cols[1])),
    l3 = strlen(_(cols[2]));
  unsigned int i = 0;
  ostringstream out;

  for (i = 0; i < l.size(); i++)
  {
    if (l1 < l[i].path.length())
      l1 = l[i].path.length();
    if (l2 < l[i].devname.length())
      l2 = l[i].devname.length();
    if (l3 < l[i].classname.length())
      l3 = l[i].classname.length();
  }

  out << _(cols[0]);
  out << spaces(2 + l1 - strlen(_(cols[0])));
  out << _(cols[1]);
  out << spaces(2 + l2 - strlen(_(cols[1])));
  out << _(cols[2]);
  out << spaces(2 + l3 - strlen(_(cols[2])));
  out << _(cols[3]);
  printline(out);

  out << spaces(l1 + l2 + l3 + strlen(_(cols[3])) + 6, "=");
  printline(out);

  for (i = 0; i < l.size(); i++)
    if (visible(l[i].classname.c_str()))
  {
    out << l[i].path;
    out << spaces(2 + l1 - l[i].path.length());
    out << l[i].devname;
    out << spaces(2 + l2 - l[i].devname.length());
    out << l[i].classname;
    out << spaces(2 + l3 - l[i].classname.length());
    out << l[i].description;
    printline(out);
  }
}


static const char *hwpathcols[] =
{
  N_("H/W path"),
  N_("Device"),
  N_("Class"),
  N_("Description")
};

void printhwpath(hwNode & node)
{
  vector < hwpath > l;
  printhwnode(node, l);
  init_wsize();
  printincolumns(l, hwpathcols);
}


static const char *businfocols[] =
{
  N_("Bus info"),
  N_("Device"),
  N_("Class"),
  N_("Description")
};

void printbusinfo(hwNode & node)
{
  vector < hwpath > l;
  printbusinfo(node, l);
  init_wsize();
  printincolumns(l, businfocols);
}
