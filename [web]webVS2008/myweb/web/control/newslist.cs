namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class newslist : UserControl
    {
        protected DataGrid DataGrid1;

        private void DataGrid1_PageIndexChanged(object source, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataBind();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            string str = "";
            if (base.Request.QueryString["type"] != null)
            {
                str = " and a.type=" + int.Parse(base.Request.QueryString["type"]).ToString();
            }
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id,a.title,a.type,author,b.name,convert(char(10),a.adddate,111) as adddate from web_news a,web_newstype b where a.type=b.id and b.used=1 " + str + " order by a.adddate desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }
    }
}
