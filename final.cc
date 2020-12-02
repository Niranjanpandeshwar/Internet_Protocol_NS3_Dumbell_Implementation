 #include <iostream>
 #include <iomanip>
 
 #include "ns3/core-module.h"
 #include "ns3/network-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/point-to-point-module.h"
 #include "ns3/applications-module.h"
 #include "ns3/traffic-control-module.h"
 
 using namespace ns3;
  
  int main (int argc, char *argv[])
  {
	  std::string tcpTypeId = "TcpDctcp";
	  std::string rate = "1Gbps"; // P2P link

          Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(10));
         // Config::SetDefault("ns3::OnOffApplication::PacketSze", UintegerValue(10));

	  
	  CommandLine cmd;
      cmd.AddValue ("tcpTypeId", "ns-3 TCP TypeId", tcpTypeId);
	  cmd.AddValue ("rate", "P2P data rate in bps", rate);
	  cmd.Parse (argc, argv);
	  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::" + tcpTypeId));
	  
	  NodeContainer c; // ALL Nodes
	  c.Create(6);

	  NodeContainer n0n4 = NodeContainer (c.Get (0), c.Get (4));
	  NodeContainer n1n4 = NodeContainer (c.Get (1), c.Get (4));
	  NodeContainer n2n5 = NodeContainer (c.Get (2), c.Get (5));
	  NodeContainer n3n5 = NodeContainer (c.Get (3), c.Get (5));
	  NodeContainer n4n5 = NodeContainer (c.Get (4), c.Get (5));
	  
	  InternetStackHelper internet;
      internet.Install (c);
	  
	  PointToPointHelper p2p;
	  p2p.SetDeviceAttribute ("DataRate", StringValue (rate));

	  NetDeviceContainer d0d4 = p2p.Install (n0n4);
	  NetDeviceContainer d1d4 = p2p.Install (n1n4);
	  NetDeviceContainer d4d5 = p2p.Install (n4n5);
	  NetDeviceContainer d2d5 = p2p.Install (n2n5);
	  NetDeviceContainer d3d5 = p2p.Install (n3n5);
	  
	  Ipv4AddressHelper ipv4;
	  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	  Ipv4InterfaceContainer i0i4 = ipv4.Assign (d0d4);

	  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
	  Ipv4InterfaceContainer i1i4 = ipv4.Assign (d1d4);

	  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
	  Ipv4InterfaceContainer i4i5 = ipv4.Assign (d4d5);

	  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
	  Ipv4InterfaceContainer i2i5 = ipv4.Assign (d2d5);

	  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
	  Ipv4InterfaceContainer i3i5 = ipv4.Assign (d3d5);
	  
	  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
	  //pakka till here.
	  // TCP connfection from N0 to N2

	  uint16_t sinkPort = 8080;
	  Address sinkAddress (InetSocketAddress (i2i5.GetAddress (0), sinkPort)); // interface of n2
	  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
	  ApplicationContainer sinkApps = packetSinkHelper.Install (c.Get (2)); //n2 as sink
	  sinkApps.Start (Seconds (0.));
	  sinkApps.Stop (Seconds (10.0));
	  
	  // Sending data
       OnOffHelper clientHelper1 ("ns3::TcpSocketFactory", Address ());
       clientHelper1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
       clientHelper1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
	   //clientHelper1.SetAttribute ("MaxPackets",UintegerValue (1));
       clientHelper1.SetAttribute ("DataRate", DataRateValue (DataRate ("1Gbps")));
       clientHelper1.SetAttribute ("PacketSize", UintegerValue (10));
 
       ApplicationContainer clientApps1;
       AddressValue remoteAddress (InetSocketAddress (i2i5.GetAddress (0), sinkPort));
       clientHelper1.SetAttribute ("Remote", remoteAddress);
       clientApps1.Add (clientHelper1.Install (c.Get (0)));  //n0 as source
       clientApps1.Start (Seconds (1.));
       clientApps1.Stop (Seconds(10.0));
	   
	   Simulator::Stop (Seconds(100.0));

		p2p.EnablePcapAll("finalpcap");
		
		  Simulator::Run ();
 
   
   Simulator::Destroy ();
   return 0;
 }
	  
	  
		
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
  
