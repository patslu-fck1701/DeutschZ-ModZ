modded class ExpansionMapMenu
{
 protected CanvasWidget m_DZADZ_RavenOverlay;

 override Widget Init()
 {
  Widget root = super.Init();
  if (root && !m_DZADZ_RavenOverlay)
  {
   m_DZADZ_RavenOverlay = CanvasWidget.Cast(GetGame().GetWorkspace().CreateWidgets("deutschz_airdropz/gui/layouts/dzadz_raven_map_overlay.layout", root));
  }
  return root;
 }

 override void Update(float timeslice)
 {
  super.Update(timeslice);
  DrawDZADZRavenSearchArea();
 }

 protected void DrawDZADZRavenSearchArea()
 {
  if (!m_DZADZ_RavenOverlay)
   return;

  m_DZADZ_RavenOverlay.Clear();
  if (!m_MapWidget || !m_ServerMarkers || !m_ServerMarkers.Contains("DZADZ_SEARCH"))
   return;

  ExpansionMapMarker centerMarker = m_ServerMarkers.Get("DZADZ_SEARCH");
  if (!centerMarker)
   return;

  ref TStringArray labelParts = new TStringArray;
  centerMarker.GetName().Split(" ", labelParts);
  if (labelParts.Count() < 2)
   return;
  float radiusWorld = labelParts[labelParts.Count() - 2].ToFloat();
  if (radiusWorld < 1)
   return;

  vector centerScreen = m_MapWidget.MapToScreen(centerMarker.GetPosition());
  vector edgeWorld = centerMarker.GetPosition() + Vector(0, 0, radiusWorld);
  vector edgeScreen = m_MapWidget.MapToScreen(edgeWorld);
  float radius = vector.Distance(Vector(centerScreen[0], centerScreen[1], 0), Vector(edgeScreen[0], edgeScreen[1], 0));
  if (radius < 2 || radius > 5000)
   return;

  // Horizontale Linien ergeben eine zoombestaendige, halbtransparente Flaeche.
  // Die Schrittweite begrenzt die Zeichenlast auch bei weit herangezoomter Karte.
  float step = Math.Max(3.0, radius / 90.0);
  for (float y = -radius; y <= radius; y += step)
  {
   float halfWidth = Math.Sqrt((radius * radius) - (y * y));
   m_DZADZ_RavenOverlay.DrawLine(centerScreen[0] - halfWidth, centerScreen[1] + y, centerScreen[0] + halfWidth, centerScreen[1] + y, step + 1.0, ARGB(58, 220, 20, 20));
  }

  // Deutlicher Rand fuer die genaue Suchgebietsgrenze.
  int segments = 72;
  for (int i = 0; i < segments; i++)
  {
   float a1 = Math.PI2 * i / segments;
   float a2 = Math.PI2 * (i + 1) / segments;
   m_DZADZ_RavenOverlay.DrawLine(centerScreen[0] + Math.Sin(a1) * radius, centerScreen[1] + Math.Cos(a1) * radius, centerScreen[0] + Math.Sin(a2) * radius, centerScreen[1] + Math.Cos(a2) * radius, 3.0, ARGB(220, 235, 25, 25));
  }
 }
}
