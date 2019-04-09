namespace web
{
    using System;
    using System.Data;
    using System.IO;
    using System.Xml;

    public class XmlControl
    {
        protected XmlDocument objXmlDoc = new XmlDocument();
        protected string strXmlFile;

        public XmlControl(string XmlFile)
        {
            try
            {
                this.objXmlDoc.Load(XmlFile);
            }
            catch (Exception exception)
            {
                throw exception;
            }
            this.strXmlFile = XmlFile;
        }

        public void Delete(string Node)
        {
            string xpath = Node.Substring(0, Node.LastIndexOf("/"));
            this.objXmlDoc.SelectSingleNode(xpath).RemoveChild(this.objXmlDoc.SelectSingleNode(Node));
        }

        public string GetAttributes(string XmlPathNode, string Attributes)
        {
            return this.objXmlDoc.SelectSingleNode(XmlPathNode).Attributes[Attributes].Value.ToString();
        }

        public string GetContent(string XmlPathNode)
        {
            return this.objXmlDoc.SelectSingleNode(XmlPathNode).InnerText;
        }

        public DataView GetData(string XmlPathNode)
        {
            DataSet set = new DataSet();
            StringReader reader = new StringReader(this.objXmlDoc.SelectSingleNode(XmlPathNode).OuterXml);
            set.ReadXml(reader);
            return set.Tables[0].DefaultView;
        }

        public void InsertElement(string MainNode, string Element, string Content)
        {
            XmlNode node = this.objXmlDoc.SelectSingleNode(MainNode);
            XmlElement newChild = this.objXmlDoc.CreateElement(Element);
            newChild.InnerText = Content;
            node.AppendChild(newChild);
        }

        public void InsertElement(string MainNode, string Element, string Attrib, string AttribContent, string Content)
        {
            XmlNode node = this.objXmlDoc.SelectSingleNode(MainNode);
            XmlElement newChild = this.objXmlDoc.CreateElement(Element);
            newChild.SetAttribute(Attrib, AttribContent);
            newChild.InnerText = Content;
            node.AppendChild(newChild);
        }

        public void InsertNode(string MainNode, string ChildNode, string Element, string Content)
        {
            XmlNode node = this.objXmlDoc.SelectSingleNode(MainNode);
            XmlElement newChild = this.objXmlDoc.CreateElement(ChildNode);
            node.AppendChild(newChild);
            XmlElement element2 = this.objXmlDoc.CreateElement(Element);
            element2.InnerText = Content;
            newChild.AppendChild(element2);
        }

        public void Save()
        {
            try
            {
                this.objXmlDoc.Save(this.strXmlFile);
            }
            catch (Exception exception)
            {
                throw exception;
            }
            this.objXmlDoc = null;
        }

        public void updateAttributes(string XmlPathNode, string Content, string Attributes)
        {
            this.objXmlDoc.SelectSingleNode(XmlPathNode).Attributes[Attributes].Value = Content;
        }

        public void updateContent(string XmlPathNode, string Content)
        {
            this.objXmlDoc.SelectSingleNode(XmlPathNode).InnerText = Content;
        }
    }
}
