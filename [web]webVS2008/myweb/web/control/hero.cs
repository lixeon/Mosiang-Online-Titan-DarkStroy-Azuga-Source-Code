namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class hero : UserControl
    {
        protected DataGrid DataGrid1;
        public int i = 1;
        public system sys;

        private void InitializeComponent()
        {
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            this.sys = new system();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select top 30 * from mhgame..tb_character  where substring(character_name,1,1)!='@' order by webchareset desc,character_grade desc,character_EXPOINT desc", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}
