namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class publicsale : UserControl
    {
        protected DataGrid DataGrid1;

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            base.Response.Redirect("psinfo.aspx?id=" + str);
        }

        private void InitializeComponent()
        {
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new system().loadConfig(0);
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_publicsale where isend=0 order by adddate desc", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}
